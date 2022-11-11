#include "Response.hpp"
#include "Client.hpp"

Response::Response(const Request &req, Config::ptr_server conf) : _req(req), _version("HTTP/1.1"), _conf(conf)
{
    try
    {
        if (_req.is_timeout())
        {
            _ge_header.connection = "close";
            sendHtmlCode(408);
        }
        else
        {
            size_t pos_slash = _req.getUrl().rfind("/");
            _conf = getConf(pos_slash);
            (this->*_map_method_ptr[req.getMethod()])();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        sendHtmlCode(500);
    }
}

Response::Response(const Response &src) : _req(src._req)
{
}

Response::~Response()
{
}

std::string Response::get_redirection(void)
{
    const std::string &url = _req.getUrl();
    const std::map<std::string, std::string> &rewrite = _conf->rewrite;
    const std::map<std::string, std::string>::const_iterator it = rewrite.find(url);
    if (it != rewrite.end())
    {
        return (it->second);
    }
    return "";
}

void Response::do_redirection(const std::string &redir)
{
    std::cout << "REDIR " << redir << std::endl;
    _re_header.location = redir;
    _status_code = 301;
}

void parse_url(const std::string &url, std::string &parse)
{
    size_t pos = url.find("?");
    if (pos == std::string::npos)
    {
        return;
    }
    parse = url.substr(pos + 1, url.length());
}

bool Response::seek_cgi(size_t pos_slash)
{
    if (!_conf)
        return (false);
    std::map<std::string, std::string>::const_iterator it(_conf->cgi_info.begin());
    std::map<std::string, std::string>::const_iterator ite(_conf->cgi_info.end());
    std::string url = getFile(pos_slash);
    std::string parse;
    std::string clean_url = url;
    size_t i = url.length();
    size_t pos = url.find("?");
    if (pos != std::string::npos)
        clean_url = url.substr(0, pos);
    while (it != ite)
    {
        while (0 < i)
        {
            if (clean_url[i] && clean_url[i] == '.')
            {
                parse = clean_url.substr(i, clean_url.length());
                if (!parse.compare(it->first))
                {
                    _cgi_path = it->second;
                    return (true);
                }
            }
            --i;
        }
        i = url.length();
        ++it;
    }
    return (false);
}

/*
    check if content type contain error code + parse it
*/
int parse_content_type_cgi(std::string const &content_type, int code)
{
    std::string parse_cnt;
    size_t pos_status = content_type.find("Status:");
    size_t pos_status2 = pos_status;
    std::stringstream iss;
    int err = 200;
    if (pos_status == std::string::npos && code != 0)
        return (500);
    else if (pos_status == std::string::npos)
        return (200);
    parse_cnt = content_type.substr(pos_status + 8, content_type.length());
    pos_status2 = parse_cnt.find(" ");
    parse_cnt.clear();
    parse_cnt = content_type.substr(pos_status + 8, pos_status2);
    std::istringstream(parse_cnt) >> err;
    return (err);
}

void Response::run_cgi_get(size_t pos_slash)
{
    if (!_conf)
        return;
    const std::string &body = _req.getBody();
    std::string url = getFile(pos_slash);
    std::string clean_url = url;
    size_t pos2 = url.find("?");
    if (pos2 != std::string::npos)
        clean_url = url.substr(0, pos2);
    const std::string path_info = clean_url;
    std::string str;
    std::stringstream ss;
    parse_url(_req.getUrl(), str);
    ss << _req.getIp().second;
    Cgi cgi(str, "CONTENT_LENGTH=" + body.length(),
            "CONTENT_TYPE=" + _req.getEntityHeader().content_type,
            "GATEWAY_INTERFACE=CGI/1.1",
            "PATH_INFO=" + path_info, "PATH_TRANSLATED=" + path_info,
            "QUERY_STRING=" + str, "REMOTE_ADDR=" + _req.getIp().first, "REMOTE_HOST=", "REQUEST_METHOD=GET",
            "SCRIPT_NAME=", "SERVER_NAME=" + _conf->server_name, "SERVER_PORT=" + ss.str(), "SERVER_PROTOCOL=" + _version);
    int code = cgi.start(_cgi_path);
    std::string ret_body(cgi.getStringStream().str());
    size_t pos = ret_body.find("\r\n\r\n");
    std::string content_type(ret_body.substr(0, pos));
    code = parse_content_type_cgi(content_type, code);
    ret_body.erase(0, pos + 4);
    fillResponse(ret_body, code, content_type);
}

void Response::run_cgi_post(size_t pos_slash)
{
    if (!_conf)
        return;
    std::string url = getFile(pos_slash);
    const std::string &body = _req.getBody();
    const std::string path_info = url;
    std::stringstream ss;
    ss << _req.getIp().second;
    Cgi cgi(body, "CONTENT_LENGTH=" + _req.getEntityHeader().content_length,
            "CONTENT_TYPE=" + _req.getEntityHeader().content_type,
            "GATEWAY_INTERFACE=CGI/1.1",
            "PATH_INFO=" + path_info, "PATH_TRANSLATED=" + path_info,
            "QUERY_STRING=", "REMOTE_ADDR=" + _req.getIp().first, "REMOTE_HOST=", "REQUEST_METHOD=POST",
            "SCRIPT_NAME=", "SERVER_NAME=" + _conf->server_name, "SERVER_PORT=" + ss.str(), "SERVER_PROTOCOL=" + _version);
    int code = cgi.start(_cgi_path);
    std::string ret_body(cgi.getStringStream().str());
    size_t pos = ret_body.find("\r\n\r\n");
    std::string content_type(ret_body.substr(0, pos));
    code = parse_content_type_cgi(content_type, code);
    ret_body.erase(0, pos + 4);
    fillResponse(ret_body, code, content_type);
}

Config::ptr_server Response::getConf(const size_t &pos_slash) const
{
    std::string url = _req.getUrl();
    if (url.find("http://") == 0)
        url = url.substr(7 + _conf->server_name.length());

    std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(
        url.substr(0, pos_slash + 1));

    return ((it != _conf->locations.end()) ? &it->second : _conf);
}

bool Response::fill_body(std::string const &file)
{
    std::cout << "file to open " << file << std::endl;
    std::ostringstream os;
    struct stat s;

    std::ifstream is(file.c_str());
    if (is && stat(file.c_str(), &s) == 0 && (s.st_mode & S_IFREG))
    {
        os << is.rdbuf();
        _status_code = 200;
        _bodyData << os.str();
        std::stringstream out;
        out << os.str().size();
        _en_header.content_length = out.str();
        size_t pos = file.rfind(".");
        if (pos != std::string::npos)
        {
            std::string mime = s_entity_header::__map_ext_mime[file.substr(pos)];
            if (mime.empty())
                _en_header.content_type = "application/octet-stream";
            else
                _en_header.content_type = mime;
        }
        else
            _en_header.content_type = "application/octet-stream";

        return true;
    }
    return false;
}

std::string Response::getFile(size_t pos_slash)
{
    const std::string &url = _req.getUrl();
    if (pos_slash == url.size() - 1)
    {
        std::vector<std::string>::const_iterator it_index;
        for (it_index = _conf->index.begin(); it_index != _conf->index.end(); ++it_index)
        {
            return (_conf->root + url + *it_index);
        }
    }
    return (_conf->root + url);
}
bool Response::handle_get(const size_t &pos_slash)
{
    const std::string &url = _req.getUrl();
    if (pos_slash == url.size() - 1)
    {
        std::vector<std::string>::const_iterator it_index;
        for (it_index = _conf->index.begin(); it_index != _conf->index.end(); ++it_index)
        {
            if (fill_body(_conf->root + url + *it_index))
                break;
        }
        if (it_index == _conf->index.end())
        {
            std::cout << "NOT FOUND\n";
            if (_conf->autoindex)
                /*if (*/sendAutoIndex(url, _conf->root + url);// == false)
                    //return sendHtmlCode(404);
            else
                return sendHtmlCode(404);
        }
    }
    else if (!fill_body(_conf->root + url))
    {
        do_redirection(url + "/");
    }
    return (true);
}

bool Response::get_method(void)
{
    std::string redir = get_redirection();
    if (!redir.empty())
        do_redirection(redir);
    else
    {
        bool is_cgi = false;
        size_t pos = _req.getUrl().rfind("/");
        if (pos == std::string::npos)
            return sendHtmlCode(404);
        is_cgi = seek_cgi(pos);
        if (is_cgi == true)
        {
            std::stringstream _iss;
            run_cgi_get(pos);
        }
        else
            handle_get(pos);
    }
    return (true);
}

std::string Response::test(const std::string &body, size_t &pos, Config::ptr_server s)
{
    std::cout << "Upload: " << s->upload_path << std::endl;
    size_t end_pos;
    std::string file;
    while ((end_pos = body.find(": ", pos)) != std::string::npos)
    {
        std::string field_name = body.substr(pos, end_pos - pos);
        transform(field_name.begin(), field_name.end(), field_name.begin(), ::tolower);
        pos = end_pos + 2;
        end_pos = body.find("\r\n", pos);
        if (end_pos == std::string::npos)
            return ("");
        std::string field_value = body.substr(pos, end_pos - pos);
        std::cout << "field name " << field_name << std::endl;
        std::cout << "field value " << field_value << std::endl;
        if (field_name == "content-disposition")
        {
            pos = field_value.find("filename=\"");
            if (pos != std::string::npos) // file to upload
            {
                pos += 10;
                size_t end = field_value.find("\"", pos + 1);
                if (end != std::string::npos)
                {
                    file = s->upload_path + field_value.substr(pos, end - pos);
                    std::cout << field_value.substr(pos, end - pos) << std::endl;
                }
            }
        }
        pos = end_pos + 2;
        if (body.find("\r\n", pos) == pos)
            break;
    }
    return file;
}
bool Response::post_method(void)
{
    bool is_cgi = false;
    size_t pos_slash = _req.getUrl().rfind("/");
    if (pos_slash == std::string::npos)
        sendHtmlCode(404);
    else

    {
        is_cgi = seek_cgi(pos_slash);
        const s_entity_header &en_header = _req.getEntityHeader();
        if (is_cgi == false)
        {
            if (en_header.content_type.find("multipart/form-data") == 0)
            {
                std::string boundary;
                size_t pos = en_header.content_type.find_first_not_of(" \t", 19);
                if (pos != std::string::npos && en_header.content_type[pos] == ';')
                {
                    pos = en_header.content_type.find_first_not_of(" \t", pos + 1);
                    if (pos != std::string::npos && en_header.content_type.find("boundary=", pos) == pos)
                    {
                        size_t end_pos = en_header.content_type.find(" \t", pos + 1);
                        boundary = en_header.content_type.substr(pos + 9, end_pos - pos);
                    }
                }
                const std::string &body = _req.getBody();
                pos = 0;
                size_t end_pos;
                while (body.find("--" + boundary + "\r\n", pos) == pos)
                {
                    pos += 4 + boundary.size();
                    end_pos = body.find("--" + boundary + "\r\n", pos);
                    if (end_pos == std::string::npos)
                    {
                        break;
                    }
                    std::cout << body.substr(pos, end_pos - pos) << std::endl;
                    std::string file = test(body, pos, _conf);

                    if (!file.empty())
                    {
                        std::ofstream ofs(file.c_str());
                        // std::cout << body.substr(pos + 2, end_pos - pos) << std::endl;
                        ofs << body.substr(pos + 2, end_pos - pos - 4);
                        ofs.close();
                    }
                    else
                        return (sendHtmlCode(400));

                    pos = end_pos;
                    // std::cout << "FUCKKKKK " << debug << " " << body.substr(pos, 15) << std::endl;
                }
            }
            handle_get(pos_slash);
        }
        else if (is_cgi == true)
        {
            run_cgi_post(pos_slash);
        }
    }
    return (true);
}

bool Response::delete_method(void)
{
    size_t pos_slash = _req.getUrl().rfind("/");
    std::string url = getFile(pos_slash);
    struct stat buffer;
    std::cout << "To delete: " << url << std::endl;
    if (stat(url.c_str(), &buffer) == 0)
    {
        if (remove(url.c_str()) == 0)
        {
            _status_code = 204;
        }
        else
            return (sendHtmlCode(403));
    }
    else
        return (sendHtmlCode(404));
    return (true);
}

std::string Response::seralize(void) const
{
    std::ostringstream oss;
    oss << _version << " " << _status_code << " " << __map_status[_status_code] << "\r\n";
    oss << _ge_header.toString();
    oss << _re_header.toString();
    oss << _en_header.toString();
    oss << "\r\n";
    oss << _bodyData.str();
    return (oss.str());
}

std::map<std::string, bool (Response::*)(void)> Response::_map_method_ptr;

void Response::init_map_method(void)
{
    _map_method_ptr["GET"] = &Response::get_method;
    _map_method_ptr["POST"] = &Response::post_method;
    _map_method_ptr["DELETE"] = &Response::delete_method;
}

bool Response::sendHtmlCode(int status_code)
{
    _bodyData << "<html> <head> <title>";
    _bodyData << status_code << " " << __map_status[status_code];
    _bodyData << "</title> </head>";
    _bodyData << "<body><center><h1>";
    _bodyData << status_code << " " << __map_status[status_code];
    _bodyData << "</h1></center><hr><center>webserv/1.0</center></body> </html>";
    std::ostringstream os;
    os << _bodyData.str().size();
    _en_header.content_length = os.str();
    _en_header.content_type = s_entity_header::__map_ext_mime[".html"];
    _status_code = status_code;
    return (false);
}

bool Response::sendAutoIndex(const std::string &uri, const std::string &directory)
{
    int code = 0;
    std::string ret;
    std::cout << "AUTOINDEX: " << directory << std::endl;
    if (load_directory_autoindex(ret, directory, uri) == false)
        code = 404;
       // return (false);
    _bodyData << ret;
    std::ostringstream os;
    os << _bodyData.str().size();
    _en_header.content_length = os.str();
    _en_header.content_type = s_entity_header::__map_ext_mime[".html"];
    if (code == 404)
        _status_code = 404;
    else
        _status_code = 200;
    return (true);
}

void Response::fillResponse(const std::string &body, int status_code, const std::string &content_type)
{
    if (status_code != 200)
    {
        sendHtmlCode(status_code);
        return;
    }
    _status_code = status_code;
    _bodyData << body;
    std::stringstream out;
    out << body.size();
    _en_header.content_length = out.str();
    _en_header.content_type = content_type;
}

std::map<int, std::string> Response::__map_status;
void Response::init_map_error()
{
    __map_status[100] = "Continue";
    __map_status[101] = "Switching Protocols";

    __map_status[200] = "OK";
    __map_status[201] = "Created";
    __map_status[202] = "Accepted";
    __map_status[203] = "Non-Authoritative Information";
    __map_status[204] = "No Content";
    __map_status[205] = "Reset Content";
    __map_status[206] = "Partial Content";

    __map_status[300] = "Multiple Choices";
    __map_status[301] = "Moved Permanently";
    __map_status[302] = "Found";
    __map_status[303] = "See Other";
    __map_status[304] = "Not Modified";
    __map_status[305] = "Use Proxy";
    __map_status[307] = "Temporary Redirect";

    __map_status[400] = "Bad Request";
    __map_status[401] = "Unauthorized";
    __map_status[402] = "Payment Required";
    __map_status[403] = "Forbidden";
    __map_status[404] = "Not Found";
    __map_status[405] = "Method Not Allowed";
    __map_status[406] = "Not Acceptable";
    __map_status[407] = "Proxy Authentication Required";
    __map_status[408] = "Request Time-out";
    __map_status[409] = "Conflict";
    __map_status[410] = "Gone";
    __map_status[411] = "Length Required";
    __map_status[412] = "Precondition Failed";
    __map_status[413] = "Request Entity Too Large";
    __map_status[414] = "Request-URI Too Large";
    __map_status[415] = "Unsupported Media Type";
    __map_status[416] = "Requested range not satisfiable";
    __map_status[417] = "Expectation Failed";

    __map_status[500] = "Internal Server Error";
    __map_status[501] = "Not Implemented";
    __map_status[502] = "Bad Gateway";
    __map_status[503] = "Service Unavailable";
    __map_status[504] = "Gateway Time-out";
    __map_status[505] = "HTTP Version Not Supported";
}

std::ostream &operator<<(std::ostream &os, const Response &rhs)
{
    os << "----response---\n";
    os << "version: " << rhs._version << std::endl;
    os << "status code: " << rhs._status_code << " " << Response::__map_status[rhs._status_code] << std::endl;
    os << rhs._ge_header.toString();
    os << rhs._re_header.toString();
    os << rhs._en_header.toString();
    os << "---response----\n";
    return (os);
}

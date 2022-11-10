#include "Response.hpp"
#include "Client.hpp"

Response::Response(const Request &req, Config::ptr_server conf) : _req(req), _version("HTTP/1.1"), _conf(conf)
{

    (this->*_map_method_ptr[req.getMethod()])();
}

Response::Response(const Response &src) : _req(src._req)
{
}

Response::~Response()
{
}

void parse_url(const std::string &url, std::string &parse)
{
    size_t i = 0;
    size_t pos = url.find("?");
    size_t start = 0;
    size_t end = 0;
    if (pos == std::string::npos)
    {
        return;
    }
    parse = url.substr(pos + 1, url.length());
}

bool Response::seek_cgi(Config::ptr_server conf, size_t pos_slash)
{

    if (!conf)
        return (false);
    std::map<std::string, std::string>::const_iterator it(conf->cgi_info.begin());
    std::map<std::string, std::string>::const_iterator ite(conf->cgi_info.end());
    std::string url = getFile(conf, pos_slash);
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

void Response::run_cgi_get(Config::ptr_server conf, size_t pos_slash)
{
    if (!conf)
        return;
    const std::string &body = _req.getBody();
    std::string url = getFile(conf, pos_slash);
    std::string clean_url = url;
    size_t pos2 = url.find("?");
    if (pos2 != std::string::npos)
        clean_url = url.substr(0, pos2);
    const std::string path_info = clean_url;
    std::string str;
    std::stringstream ss;
    parse_url(_req.getUrl(), str);
    ss << _req.getIp().second;
    std::cout << "CGI GET" << std::endl;
    std::cout << "PATH_info:" << path_info << std::endl;
    Cgi cgi("", "CONTENT_LENGTH=" + body.length(),
            "CONTENT_TYPE=" + _req.getEntityHeader().content_type,
            "GATEWAY_INTERFACE=CGI/1.1",
            "PATH_INFO=" + clean_url, "PATH_TRANSLATED=" + path_info,
            "QUERY_STRING=" + str, "REMOTE_ADDR=" + _req.getIp().first, "REMOTE_HOST=", "REQUEST_METHOD=GET",
            "SCRIPT_NAME=", "SERVER_NAME=" + conf->server_name, "SERVER_PORT=" + ss.str(), "SERVER_PROTOCOL=" + _version);
    cgi.start(_cgi_path);
    std::string ret_body(cgi.getStringStream().str());
    size_t pos = ret_body.find("\r\n\r\n");
    std::string content_type(ret_body.substr(0, pos));
    ret_body.erase(0, pos + 4);
    fillResponse(ret_body, 200, content_type);
}

/*
    check if content type contain error code + parse it
*/
int parse_content_type_cgi(std::string const &content_type)
{
    std::string parse_cnt;
    std::cout << "cnt:" << content_type << std::endl;
    size_t pos_status = content_type.find("Status:");
    size_t pos_status2 = pos_status;
    std::stringstream iss;
    int err = 200;
    if (pos_status == std::string::npos)
        return (200);
    parse_cnt = content_type.substr(pos_status + 8, content_type.length());
    pos_status2 = parse_cnt.find(" ");
    parse_cnt.clear();
    parse_cnt = content_type.substr(pos_status + 8, pos_status2);
    std::istringstream(parse_cnt) >> err;
    return (err);
}

void Response::run_cgi_post(Config::ptr_server conf, size_t pos_slash)
{
    if (!conf)
        return;
    std::string url = getFile(conf, pos_slash);
    const std::string &body = _req.getBody();
    const std::string path_info = url;
    std::stringstream ss;
    ss << _req.getIp().second;
    Cgi cgi(body, "CONTENT_LENGTH=" + _req.getEntityHeader().content_length,
            "CONTENT_TYPE=" + _req.getEntityHeader().content_type,
            "GATEWAY_INTERFACE=CGI/1.1",
            "PATH_INFO=" + url, "PATH_TRANSLATED=" + path_info,
            "QUERY_STRING=", "REMOTE_ADDR=" + _req.getIp().first, "REMOTE_HOST=", "REQUEST_METHOD=POST",
            "SCRIPT_NAME=", "SERVER_NAME=" + conf->server_name, "SERVER_PORT=" + ss.str(), "SERVER_PROTOCOL=" + _version);
    cgi.start(_cgi_path);
    std::string ret_body(cgi.getStringStream().str());
    size_t pos = ret_body.find("\r\n\r\n");
    std::string content_type(ret_body.substr(0, pos));
    int code = parse_content_type_cgi(content_type);
    ret_body.erase(0, pos + 4);
    fillResponse(ret_body, code, content_type);
}

Config::ptr_server Response::getConf(const size_t &pos_slash) const
{
    std::string url = _req.getUrl();
    if (url.find("http://") == 0)
        url = url.substr(7 + _conf->server_name.length());
    std::cout << "pos slash " << pos_slash << std::endl;
    std::cout << "sustring " << url.substr(0, pos_slash + 1) << std::endl;
    std::cout << "server " << _conf->server_name << std::endl;

    std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(
        url.substr(0, pos_slash + 1));

    return ((it != _conf->locations.end()) ? &it->second : _conf);
}

bool Response::fill_body(std::string const &file)
{
    std::cout << "file to open " << file << std::endl;
    std::ostringstream os;
    std::ifstream is(file.c_str());
    if (is)
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
                std::cout << "mime dont exist (request error)\n";
            else
                _en_header.content_type = mime;
        }
        else
            _en_header.content_type = "application/octet-stream";

        return true;
    }
    return false;
}

std::string Response::getFile(Config::ptr_server s, size_t pos_slash)
{
    const std::string &url = _req.getUrl();
    if (pos_slash == url.size() - 1)
    {
        std::vector<std::string>::const_iterator it_index;
        for (it_index = s->index.begin(); it_index != s->index.end(); ++it_index)
        {
            return (s->root + url + *it_index);
        }
    }
    return (s->root + url);
}
void Response::handle_get(Config::ptr_server s, const size_t &pos_slash)
{
    const std::string &url = _req.getUrl();
    if (pos_slash == url.size() - 1)
    {
        std::vector<std::string>::const_iterator it_index;
        for (it_index = s->index.begin(); it_index != s->index.end(); ++it_index)
        {
            if (fill_body(s->root + url + *it_index))
                break;
        }
        if (it_index == s->index.end())
        {
            std::cout << "NOT FOUND\n";
            if (s->autoindex)
                sendAutoIndex(url, s->root + url);
            else
                sendHtmlCode(404, s);
        }
    }
    else if (!fill_body(s->root + url))
        sendHtmlCode(404, s);
}

void Response::get_method(void)
{
    bool is_cgi = false;
    size_t pos = _req.getUrl().rfind("/");
    if (pos == std::string::npos)
        std::cout << "404" << std::endl;
    Config::ptr_server s = getConf(pos);
    is_cgi = seek_cgi(s, pos);
    if (is_cgi == true)
    {
        std::stringstream _iss;
        run_cgi_get(s, pos);
        std::cout << "iss:" << _iss.str() << std::endl;
    }
    else
        handle_get(s, pos);
}

std::string Response::test(const std::string &body, size_t &pos, Config::ptr_server s)
{
    std::cout << "upload: " << s->upload_path << std::endl;

    size_t end_pos;
    std::string file;
    while ((end_pos = body.find(": ", pos)) != std::string::npos)
    {
        std::string field_name = body.substr(pos, end_pos - pos);
        transform(field_name.begin(), field_name.end(), field_name.begin(), ::tolower);
        pos = end_pos + 2;
        end_pos = body.find("\r\n", pos);
        if (end_pos == std::string::npos)
            std::cout << "bad body multipart" << std::endl;
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
                    std::cout << "end " << end << std::endl;
                    std::cout << "pos " << pos << std::endl;
                    std::cout << field_value.substr(pos, end - pos) << std::endl;
                    std::cout << "file: " << file << std::endl;
                    std::cout << "upload: " << s->upload_path << std::endl;
                }
                else
                    std::cout << "error" << std::endl;
            }
        }
        pos = end_pos + 2;
        if (body.find("\r\n", pos) == pos)
            break;
    }
    return file;
}
void Response::post_method(void)
{
    bool is_cgi = false;
    size_t pos_slash = _req.getUrl().rfind("/");
    if (pos_slash == std::string::npos)
        std::cout << "404" << std::endl;
    Config::ptr_server s = getConf(pos_slash);
    is_cgi = seek_cgi(s, pos_slash);
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
                    std::cout << "boundary " << boundary << std::endl;
                }
            }
            const std::string &body = _req.getBody();
            pos = 0;
            size_t end_pos;
            // std::cout << "here" << s << std::endl;
            while (body.find("--" + boundary + "\r\n", pos) == pos)
            {
                pos += 4 + boundary.size();
                end_pos = body.find("--" + boundary + "\r\n", pos);
                if (end_pos == std::string::npos)
                {
                    std::cout << "777777777777777777777777777777777777777777777777777777\n";
                    break;
                }
                std::cout << "BODY: " << std::endl;
                std::cout << body.substr(pos, end_pos - pos) << std::endl;
                std::string file = test(body, pos, s);

                if (!file.empty())
                {
                    std::ofstream ofs(file.c_str());
                    std::cout << "IMAGE:" << file << std::endl;
                    // std::cout << body.substr(pos + 2, end_pos - pos) << std::endl;
                    ofs << body.substr(pos + 2, end_pos - pos - 4);
                    ofs.close();
                }
                pos = end_pos;

                // std::cout << "FUCKKKKK " << debug << " " << body.substr(pos, 15) << std::endl;
            }
        }
        handle_get(s, pos_slash);
    }
    else if (is_cgi == true)
    {
        run_cgi_post(s, pos_slash);
        // std::cout << "iss:" << _iss.str() << std::endl;
    }
}

void Response::delete_method(void)
{
    std::string url = _req.getUrl();
    struct stat buffer;
    if (stat(url.c_str(), &buffer) == 0)
        _status_code = remove(url.c_str()) == 0 ? 204 : 403;
    else
        _status_code = 404;
}

std::string Response::seralize(void) const
{
    std::ostringstream oss;
    oss << _version << " " << _status_code << " OK\r\n";
    oss << _ge_header.toString();
    oss << _re_header.toString();
    oss << _en_header.toString();
    oss << "\r\n";
    oss << _bodyData.str();
    return (oss.str());
}

std::map<std::string, void (Response::*)(void)> Response::_map_method_ptr;

void Response::init_map_method(void)
{
    _map_method_ptr["GET"] = &Response::get_method;
    _map_method_ptr["POST"] = &Response::post_method;
    _map_method_ptr["DELETE"] = &Response::delete_method;
}

void Response::sendHtmlCode(int status_code, Config::ptr_server s)
{

    _bodyData << "<html> <head> <title>";
    _bodyData << status_code << " " << __map_error[status_code];
    _bodyData << "</title> </head>";
    _bodyData << "<body>";
    _bodyData << status_code << " " << __map_error[status_code];
    _bodyData << "</body> </html>";
    std::ostringstream os;
    os << _bodyData.str().size();
    _en_header.content_length = os.str();
    _en_header.content_type = s_entity_header::__map_ext_mime[".html"];
    _status_code = status_code;
}

void Response::sendAutoIndex(const std::string &uri, const std::string &directory)
{
    std::string ret;
    std::cout << "AUTOINDEX " << directory << std::endl;
    load_directory_autoindex(ret, directory, uri);
    _bodyData << ret;
    std::ostringstream os;
    os << _bodyData.str().size();
    _en_header.content_length = os.str();
    _en_header.content_type = s_entity_header::__map_ext_mime[".html"];
    _status_code = 200;
}

void Response::fillResponse(const std::string &body, int status_code, const std::string &content_type)
{
    _status_code = status_code;
    _bodyData << body;
    std::stringstream out;
    out << body.size();
    _en_header.content_length = out.str();
    _en_header.content_type = content_type;
}

std::map<int, std::string> Response::__map_error;
void Response::init_map_error()
{
    __map_error[100] = "Continue";
    __map_error[101] = "Switching Protocols";

    __map_error[200] = "OK";
    __map_error[201] = "Created";
    __map_error[202] = "Accepted";
    __map_error[203] = "Non-Authoritative Information";
    __map_error[204] = "No Content";
    __map_error[205] = "Reset Content";
    __map_error[206] = "Partial Content";

    __map_error[300] = "Multiple Choices";
    __map_error[301] = "Moved Permanently";
    __map_error[302] = "Found";
    __map_error[303] = "See Other";
    __map_error[304] = "Not Modified";
    __map_error[305] = "Use Proxy";
    __map_error[307] = "Temporary Redirect";

    __map_error[400] = "Bad Request";
    __map_error[401] = "Unauthorized";
    __map_error[402] = "Payment Required";
    __map_error[403] = "Forbidden";
    __map_error[404] = "Not Found";
    __map_error[405] = "Method Not Allowed";
    __map_error[406] = "Not Acceptable";
    __map_error[407] = "Proxy Authentication Required";
    __map_error[408] = "Request Time-out";
    __map_error[409] = "Conflict";
    __map_error[410] = "Gone";
    __map_error[411] = "Length Required";
    __map_error[412] = "Precondition Failed";
    __map_error[413] = "Request Entity Too Large";
    __map_error[414] = "Request-URI Too Large";
    __map_error[415] = "Unsupported Media Type";
    __map_error[416] = "Requested range not satisfiable";
    __map_error[417] = "Expectation Failed";

    __map_error[500] = "Internal Server Error";
    __map_error[501] = "Not Implemented";
    __map_error[502] = "Bad Gateway";
    __map_error[503] = "Service Unavailable";
    __map_error[504] = "Gateway Time-out";
    __map_error[505] = "HTTP Version Not Supported";
}
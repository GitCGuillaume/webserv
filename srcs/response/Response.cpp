#include "Response.hpp"

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

void Response::get_method(void)
{
    std::string url = _req.getUrl();
    if (url.find("http://") == 0)
        url = url.substr(7 + _conf->server_name.length());
    size_t pos = url.rfind("/");
    if (pos == std::string::npos)
        std::cout << "404" << std::endl;
    std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(url.substr(0, pos + 1));
    Config::ptr_server s = (it != _conf->locations.end()) ? &it->second : _conf;
    if (pos == url.size() - 1)
    {
        std::vector<std::string>::const_iterator it_index;
        for (it_index = s->index.begin(); it_index != s->index.end(); ++it_index)
        {
            if (fill_body(s->root + url + *it_index))
                break;
        }
        if (it_index == s->index.end())
            std::cout << "404" << std::endl;
    }
    else if (!fill_body(s->root + url))
        std::cout << "404" << std::endl;
}

void Response::test(const std::string &s, size_t pos)
{
    size_t end_pos;
    while ((end_pos = s.find(": ", pos)) != std::string::npos)
    {
        std::string field_name = s.substr(pos, end_pos - pos);
        transform(field_name.begin(), field_name.end(), field_name.begin(), ::tolower);
        pos = end_pos + 2;
        end_pos = s.find("\r\n", pos);
        if (end_pos == std::string::npos)
            std::cout << "bad body multipart" << std::endl;
        std::string field_value = s.substr(pos, end_pos - pos);
        std::cout << field_name << std::endl;
        std::cout << field_value << std::endl;
        if (field_name == "content-disposition")
        {
            pos = field_value.find("filename=\"");
            if (pos != std::string::npos) // file to upload
            {
                pos += 10;
                end_pos = field_value.find("\"", pos + 1);
                std::string file = field_value.substr(pos, end_pos - pos);
                // std::ofstream ofs (file, std::ofstream::out);
                std::cout << "file: " << file << std::endl;
            }
        }
    }
}
void Response::post_method(void)
{
    get_method();
    const s_entity_header &en_header = _req.getEntityHeader();
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
        const std::string &s = _req.getBody();
        pos = 0;
        size_t end_pos;
        // std::cout << "here" << s << std::endl;
        while (s.find("--" + boundary + "\r\n", pos) == pos)
        {
            pos += 4 + boundary.size();
            test(s, pos);
            end_pos = s.find("--" + boundary + "\r\n", pos);
            // std::cout << s.substr(pos, end_pos - pos) << std::endl;
            pos = end_pos + 1;
        }
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

std::string Response::display_error(int status_code, std::string const &error_page) {

    std::ostringstream res;
    res << "<html> <head> <title>";
    res << status_code << " " << __map_error[status_code];
    res << "</title> </head>";
    res << "<body>";
    res<< status_code << " "<<__map_error[status_code];
    res << "</body> </html>";
    return res.str();
}

std::map<int, std::string> Response::__map_error;
void Response::init_map_error()
{
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
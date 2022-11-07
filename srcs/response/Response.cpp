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

Config::ptr_server Response::getConf(const size_t &pos_slash) const
{
    std::string url = _req.getUrl();
    if (url.find("http://") == 0)
        url = url.substr(7 + _conf->server_name.length());
    std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(url.substr(0, pos_slash + 1));
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
            std::cout << "404" << std::endl;
    }
    else if (!fill_body(s->root + url))
        std::cout << "404" << std::endl;
}

void Response::get_method(void)
{
    size_t pos = _req.getUrl().rfind("/");
    if (pos == std::string::npos)
        std::cout << "404" << std::endl;
    Config::ptr_server s = getConf(pos);
    handle_get(s, pos);
}

std::string Response::test(const std::string &body, size_t &pos, Config::ptr_server s)
{
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
        std::cout << "field value " <<field_value << std::endl;
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
                    std::cout << "file: " << file << std::endl;
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
    int debug = 0;
    size_t pos_slash = _req.getUrl().rfind("/");
    if (pos_slash == std::string::npos)
        std::cout << "404" << std::endl;
    Config::ptr_server s = getConf(pos_slash);
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
        const std::string &body = _req.getBody();
        pos = 0;
        size_t end_pos;
        // std::cout << "here" << s << std::endl;
        while (body.find("--" + boundary + "\r\n", pos) == pos)
        {
            debug++;
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
                std::ofstream ofs (file.c_str());
                //std::cout << "IMAGE:" << std::endl;
                //std::cout << body.substr(pos + 2, end_pos - pos) << std::endl;
                ofs << body.substr(pos + 2, end_pos - pos - 4);
                ofs.close();

            }
            pos = end_pos;

            //std::cout << "FUCKKKKK " << debug << " " << body.substr(pos, 15) << std::endl;
        }
    }
    handle_get(s, pos_slash);
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

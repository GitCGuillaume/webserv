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

bool Response::fill_body(Config::ptr_server s, std::string const &file)
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
        return true;
    }
    return false;
}





// void Response::get_method(void)
// {
//     const std::string &url = _req.getUrl();
//     if (url.find("http://") == 0)
//         handle_index(url.substr(7 + _conf->server_name.length()));
//     else 
//         handle_index(url);
// }

// void Response::handle_index(const std::string &url) {
//         size_t pos = url.rfind("/");
//         if (pos == std::string::npos)
//              std::cout << "404" << std::endl; 
//         std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(url.substr(0, pos + 1));
//         if (it != _conf->locations.end()) {
//             Config::ptr_server s = &it->second;
//             if (pos == url.size() -1) {
//                 std::vector<std::string>::const_iterator it_index;
//                 for (it_index = s->index.begin(); it_index != s->index.end(); ++it_index) {
//                     if (fill_body(s, s->root + url + *it_index))
//                         break;
//                 }
//                 if (it_index == s->index.end())
//                     std::cout << "404" << std::endl;
//             } 
//             else if (!fill_body(s, s->root + url))
//                 std::cout << "404" << std::endl;
//         }
//         else if (!fill_body(_conf, _conf->root + url))
//             std::cout << "404" << std::endl;
// }


void Response::get_method(void)
{
    Config::ptr_server s;
    const std::string &url = _req.getUrl();
    if (url.find("http://") == 0) // absolute
    {
        std::cout << "server name" << _conf->server_name <<std::endl;
        std::string file = url.substr(7 + _conf->server_name.length());
        size_t pos = file.rfind("/");

        if (pos == url.size() - 1) // go search index
        {

            std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(file);
            if (it != _conf->locations.end())
            {
                s = &it->second;
                std::vector<std::string>::const_iterator it_index;
                for (it_index = s->index.begin(); it_index != s->index.end(); ++it_index)
                {
                    if (fill_body(s, s->root + file + *it_index))
                        break;
                }
                if (it_index == s->index.end())
                    std::cout << "404" << std::endl;
            }
        }
        else if (pos != std::string::npos) // no index
        {               
            std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(file.substr(0, pos + 1));
            if (it != _conf->locations.end())
            {
                s = &it->second;
                if (!fill_body(s, s->root + file))
                    std::cout << "404" << std::endl;
            }
            else
            {
                if (!fill_body(_conf,  _conf->root + file))
                    std::cout << "404" << std::endl;
            }
        }
        else // nerver happens
            std::cout << "404" << std::endl; 
    }
    else
    {
        size_t pos = url.rfind("/");
        if (pos == url.size() - 1) // go search index
        {
            std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(url);
            if (it != _conf->locations.end())
            {
                s = &it->second;
                std::vector<std::string>::const_iterator it_index;
                for (it_index = s->index.begin(); it_index != s->index.end(); ++it_index)
                {
                    if (fill_body(s, s->root + url + *it_index))
                        break;
                }
                if (it_index == s->index.end())
                    std::cout << "404" << std::endl;
            }
        }
        else if (pos != std::string::npos) // no index
        {
            std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(url.substr(0, pos + 1));
            if (it != _conf->locations.end())
            {
                s = &it->second;
                if (!fill_body(s, s->root + url))
                    std::cout << "404" << std::endl;
            }
            else
            {
                if (!fill_body(_conf, _conf->root + url))
                    std::cout << "404" << std::endl;
            }
        }
        else // nerver happens
            std::cout << "404" << std::endl; 

    }
}

void Response::post_method(void)
{
    _status_code = 200;
    _en_header.content_length = "17";
    _bodyData << "Hey ! It's a test";
}

void Response::delete_method(void)
{
    _status_code = 200;
    _en_header.content_length = "17";
    _bodyData << "Hey ! It's a test";
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

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

void Response::get_method(void)
{
    
    std::cout << "HERE "<<_conf->server_name << std::endl;
    std::cout << "URL " << _req.getUrl() <<"|" << std::endl;
    std::cout << *_conf << std::endl;
    std::map<std::string, Config::server>::const_iterator it = _conf->locations.find(_req.getUrl());
    if (it != _conf->locations.end())
    {
        Config::ptr_server s = &it->second;
        std::string file = s->root + _req.getUrl();
        std::cout << "file to open " << file << std::endl;
        _status_code = 200;
        _en_header.content_length = "17";
        _bodyData << "Hey ! It's a test";
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

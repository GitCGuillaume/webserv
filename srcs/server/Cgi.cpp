#include <Cgi.hpp>

Cgi::Cgi() : _content_type(""), _gateway_interface(""), _path_info(""),
    _path_translated(""), _query_string(""), _remote_addr(""), _remote_host(""),
    _request_method(""), _script_name(""), _server_name(""), _server_protocol(""),
    _content_length(""), _server_port(""){}

Cgi::Cgi(std::string & content_type , std::string & gateway_interface,
            std::string & path_info, std::string & path_translated, std::string & query_string,
            std::string & remote_addr, std::string & remote_host, std::string & request_method,
            std::string & script_name, std::string & server_name, std::string & server_protocol,
            std::string & content_length, std::string & server_port) : _content_type(content_type), _gateway_interface(gateway_interface), _path_info(path_info),
    _path_translated(path_translated), _query_string(query_string), _remote_addr(remote_addr), _remote_host(remote_host),
    _request_method(request_method), _script_name(script_name), _server_name(server_name), _server_protocol(server_protocol),
    _content_length(content_length), _server_port(server_port){}
Cgi::~Cgi(){}
Cgi::Cgi(Cgi const & src) : _content_type(src._content_type), _gateway_interface(src._gateway_interface), _path_info(src._path_info),
    _path_translated(src._path_translated), _query_string(src._query_string), _remote_addr(src._remote_addr), _remote_host(src._remote_host),
    _request_method(src._request_method), _script_name(src._script_name), _server_name(src._server_name), _server_protocol(src._server_protocol),
    _content_length(src._content_length), _server_port(src._server_port)
{
}
//Cgi & Cgi::operator=(Cgi const & src)
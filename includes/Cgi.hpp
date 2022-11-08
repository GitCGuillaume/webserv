#ifndef CGI_PHP
#define CGI_PHP

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <sstream>
#include <fstream>

class Cgi
{
private:
    /*std::string _content_type;
    std::string _gateway_interface;
    std::string _path_info;
    std::string _path_translated;
    std::string _query_string;
    std::string _remote_addr;
    std::string _remote_host;
    std::string _request_method;
    std::string _script_name;
    std::string _server_name;
    std::string _server_protocol;
    std::string _content_length;
    std::string _server_port;*/
    std::vector<std::string> _vec;
    std::string const _body;
    std::stringstream _iss;

public:
    Cgi();
    explicit Cgi(std::string const body, std::string const content_length, std::string const content_type,
                 std::string const gateway_interface, std::string const path_info,
                 std::string const path_translated, std::string const query_string,
                 std::string const remote_addr, std::string const remote_host, std::string const request_method,
                 std::string const script_name, std::string const server_name, std::string const server_port,
                 std::string const server_protocol);
    ~Cgi();
    Cgi(Cgi const &src);
    Cgi &operator=(Cgi const &src);
    const std::stringstream &getStringStream() const;
    void start();
};

#endif
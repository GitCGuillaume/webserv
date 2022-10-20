#ifndef CGI_PHP
# define CGI_PHP

#include <iostream>

class Cgi
{
    private:
        std::string _content_type;
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
        std::string _server_port;

    public:
        Cgi();
        explicit Cgi(std::string & content_type, std::string & gateway_interface,
            std::string & path_info, std::string & path_translated, std::string & query_string,
            std::string & remote_addr, std::string & remote_host, std::string & request_method,
            std::string & script_name, std::string & server_name, std::string & server_protocol,
            std::string & content_length, std::string & server_port);
        ~Cgi();
        Cgi(Cgi const & src);
        //Cgi & operator=(Cgi const & src);
};

#endif
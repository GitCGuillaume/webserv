#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <sys/stat.h>
#include <iostream>
#include <errno.h>
#include "Request.hpp"
#include <cstring>
#include <cstdio>

class Response
{
private:
    const Request &_req;
    std::string _version;
    int _status_code;
    s_general_header _ge_header;
    s_response_header _re_header;
    s_entity_header _en_header;
    std::ostringstream _bodyData;
    Config::ptr_server _conf;


    static std::map<std::string, void (Response::*)(void)> _map_method_ptr;

public:
    Response(const Request &req, Config::ptr_server conf);
    Response(const Response &src);
    ~Response();

    void get_method(void);
    void post_method(void);
    void delete_method (void);
    bool fill_body(std::string const &file);
    void test(const std::string &s, size_t pos);
    std::string seralize(void) const;
    static void init_map_method(void);


};

#endif
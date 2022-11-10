#ifndef HEADER_HPP
#define HEADER_HPP
#include <iostream>
#include <map>

struct s_header
{
};
struct s_general_header : public s_header
{
    std::string date;
    std::string transfer_encoding;

    std::string toString(void) const;

    static std::map<std::string, std::string s_general_header::*> __map_ge_headers_ptr;
    static void init_map_ge_headers(void);
};
struct s_request_header : public s_header
{
    std::string host;

    std::string toString(void) const;

    static std::map<std::string, std::string s_request_header::*> _map_re_headers_ptr;
    static void init_map_re_headers(void);
};

struct s_response_header : public s_header
{
    std::string toString(void) const;

    std::string etag;
    std::string server;
    std::string location;
};
struct s_entity_header : public s_header
{
    std::string content_type;
    std::string content_length;

    std::string toString(void) const;

    static std::map<std::string, std::string s_entity_header::*> __map_en_headers_ptr;
    static std::map<std::string, std::string> __map_mime_ext;
    static std::map<std::string, std::string> __map_ext_mime;
    static void init_map_en_headers(void);
    static void init_map_mime(void);
};

#endif
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "upload_tools.hpp"
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>

class Request
{

public:
    typedef struct s_header
    {
        std::string content_type;
        long content_length;
        std::string date;
        std::string host;
        std::string transfer_encoding;
        s_header() : content_length(-1) {}
    } t_header;
    Request();
    Request(const std::string &req);
    Request(const Request &src);
    ~Request();

    static bool is_method(const std::string &str);
    void init_map_headers(void);

    size_t parse(void);
    size_t parse_request_line(void);
    size_t parse_header(size_t start);
    size_t parse_body(size_t start);
    void reset(void);
    void append_data(const char *data, size_t n);

    Request &operator=(const Request &rhs);
    Request &operator=(const std::string &rhs);
    Request &operator+=(const char *rhs);
    friend std::ostream &operator<<(std::ostream &os, const Request &rhs);

    const std::string &getReq(void);
    bool is_ready() const;
    const t_header &getHeader() const;
    size_t size();

private:
    std::string _req;
    std::stringstream ss;

    std::string _method;
    std::string _url;
    std::string _version;

    std::map<std::string, void *> _map_headers;
    t_header _header;
    bool _is_ready;
    int _code;
};
#endif

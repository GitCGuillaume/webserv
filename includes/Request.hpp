#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include "Header.hpp"
#include "Config.hpp"

class Request
{

public:
    // typedef struct s_header
    // {
    //     std::string content_type;
    //     std::string content_length;
    //     std::string date;
    //     std::string host;
    //     std::string transfer_encoding;
    // } t_header;
    Request();
    Request(const std::string &req);
    Request(const Request &src);
    ~Request();

    static bool is_method(const std::string &str);

    size_t parse(void);
    size_t parse_request_line(void);
    size_t parse_header(size_t start);
    size_t parse_body(size_t start);
    void reset(void);
    void append_data(const char *data, size_t n);
    bool fillHeader(const std::string &field_name, const std::string &field_value);

    Request &operator=(const Request &rhs);
    Request &operator=(const std::string &rhs);
    Request &operator+=(const char *rhs);
    friend std::ostream &operator<<(std::ostream &os, const Request &rhs);

    const std::string &getReq(void);
    const std::string &getMethod(void) const;
    const std::string &getUrl(void) const;
    const std::string &getVersion(void) const;
    bool is_ready() const;
    const s_entity_header &getEntityHeader() const;
    //const t_header &getHeader() const;
    size_t getContentLength() const;
    size_t size();

private:
    std::string _req;
    std::stringstream ss;

    std::string _method;
    std::string _url;
    std::string _version;

    //t_header _header;
    s_general_header _ge_header;
    s_request_header _re_header;
    s_entity_header _en_header;
    bool _is_ready;
    int _code;
};
#endif

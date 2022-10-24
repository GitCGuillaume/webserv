#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <set>
#include <map>
#include <algorithm>
class Request
{

private:
    std::string _req;

    std::string _method;
    std::string _url;
    std::string _version;
    typedef struct s_header
    {
        std::string content_type;
        size_t content_length;
        std::string date;
        std::string host;
        std::string transfer_encoding;
        s_header() : content_length(0) {}
    } t_header;
    std::map<std::string, void *> _map_headers;
    t_header _header;
    bool _is_ready;
    int _code;

public:
    Request();
    Request(const std::string &req);
    Request(const Request &src);
    ~Request();

    static bool is_method(const std::string &str);
    void init_map_headers(void);

    void parse(void);
    size_t parse_request_line(void);
    size_t parse_header(size_t start);
    void reset(void);

    Request &operator=(const Request &rhs);
    Request &operator=(const std::string &rhs);
    Request &operator+=(const std::string &rhs);
    friend std::ostream &operator<<(std::ostream &os, const Request &rhs);

    const std::string &getReq(void) const;
    bool is_ready() const;
};
#endif

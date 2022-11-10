#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include <sys/time.h>
#include "Header.hpp"
#include "Config.hpp"
//#include "Client.hpp" //ERROR

class Client;
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
    Request(const Client &Client);
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
    const std::string &getBody() const;
    const Client &getClient() const;
    const std::pair<std::string,uint16_t> &getIp() const;
    size_t size();
    timeval get_time() const;
    void set_time(timeval &tv);



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
    std::string _body;
    bool _is_ready;
    int _code;
    const Client &_client;

	timeval _time;
};
#endif

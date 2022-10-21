#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <set>
class Request
{
    private:
        std::string _req;

        enum e_method
        {
            GET, POST, DELETE
        };
        e_method method;
        std::string url;
        std::string version;
        typedef struct s_header
        {
            std::string content_type;
            size_t content_length;
            std::string date;
            std::string host;
            std::string transfer_encoding;
        }t_header;
        t_header header;
    public:
        Request();
        Request(const std::string  &req);
        Request(const Request &src);
        ~Request();

        static bool is_method(const std::string &str);

        Request &operator=(const Request &rhs);
        Request &operator=(const std::string &rhs);
        Request &operator+=(const std::string &rhs);
        friend std::ostream &operator<<(std::ostream &os, const Request &rhs);

        const std::string &getReq(void) const;
        
};
#endif

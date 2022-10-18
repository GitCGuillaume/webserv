#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
class Request
{
    private:
        std::string _req;
    public:
        Request();
        Request(const std::string  &req);
        Request(const Request &src);
        ~Request();

        Request &operator=(const Request &rhs);
        Request &operator=(const std::string &rhs);
        Request &operator+=(const std::string &rhs);
        friend std::ostream &operator<<(std::ostream &os, const Request &rhs);

        const std::string &getReq(void) const;
};
#endif

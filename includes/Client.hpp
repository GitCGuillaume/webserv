#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <stdint.h>
//#include "Request.hpp"
//#include "Response.hpp"
#include "ServerInput.hpp"
#include "ServerOutput.hpp"
#include "Config.hpp"

class Client
{
    public:
        typedef std::pair<std::string,uint16_t> host_type;

        Client(host_type ip, int socket, Config::ptr_server conf);
        Client(const Client &src);
        ~Client();

        Client &operator=(const Client &rhs);

        void    epoll_in(void);
        void    epoll_out(void);

        const host_type &getIp(void) const;

        Config::ptr_server get_conf() const;
        const Request &getReq() const;
    
    private:
        host_type _ip;
        Config::ptr_server _conf;
        ServerInput _si;
        ServerOutput _so;
};
#endif

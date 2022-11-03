#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <stdint.h>
#include "ServerInput.hpp"
#include "ServerOutput.hpp"
#include "Config.hpp"

class Client
{
    private:
        uint16_t _port;
        Config::ptr_server _conf;
        ServerInput _si;
        ServerOutput _so;
    public:
        Client(uint16_t port, int socket, Config::ptr_server conf);
        Client(const Client &src);
        ~Client();

        Client &operator=(const Client &rhs);

        void    epoll_in(void);
        void    epoll_out(void);

        int getPort(void) const;
};
#endif

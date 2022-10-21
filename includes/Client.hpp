#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <stdint.h>
#include "ServerInput.hpp"
#include "ServerOutput.hpp"

class Client
{
    private:
        uint16_t _port;
        ServerInput _si;
        ServerOutput _so;
    public:
        Client(uint16_t port, int socket);
        Client(const Client &src);
        ~Client();

        Client &operator=(const Client &rhs);

        void    epoll_in(void);

        int getPort(void) const;
};
#endif

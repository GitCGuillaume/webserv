#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <stdint.h>
#include "ServerInput.hpp"
#include "ServerOutput.hpp"
#include "Config.hpp"

class Client
{
private:
    std::pair<std::string, uint16_t> _ip;
    Config::ptr_server _conf;
    ServerInput _si;
    ServerOutput _so;

public:
    typedef std::pair<std::string, uint16_t> host_type;

    Client(host_type ip, int socket, Config::ptr_server conf);
    Client(const Client &src);
    ~Client();

    Client &operator=(const Client &rhs);

    void epoll_in(void);
    void epoll_out(void);

    int getPort(void) const;
};
#endif

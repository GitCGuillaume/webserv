#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <stdint.h>
class Client
{
    private:
        uint16_t _port;
        int _socket;
    public:
        Client(uint16_t port, int socket);
        Client(const Client &src);
        ~Client();

        Client &operator=(const Client &rhs);

        void    sendResponse(void);

        int getPort(void) const;
        int getSocket(void) const;
};
#endif

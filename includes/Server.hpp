#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include "Client.hpp"

class Server
{
    private:
        std::vector<int> _sockets;
        int _socket;
        std::vector<Client> _clients;
    
    public:
        Server();
        ~Server();
        Server &    operator=(Server const cpy);
        Server(Server const & cpy);
        int const & getSocket() const;
        void setSocket(int const domain, int const type, int protocol);
};

#endif
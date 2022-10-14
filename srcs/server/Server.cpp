#include "Server.hpp"

Server::Server() : _socket(0){}

Server::~Server() {}

Server &    Server::operator=(Server const cpy)
{
    if (this != &cpy)
    {
        _socket = cpy._socket;
    }
    return (*this);
}
Server::Server(Server const & cpy) : _socket(cpy._socket)
{
}

int const & Server::getSocket() const
{
    return (_socket);
}

void Server::setSocket(int const domain, int const type, int protocol)
{
    _socket = socket(domain, type, protocol);
}

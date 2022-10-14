#include "Server.hpp"

Server::Server() : _socket(0), _bind(0), _listen(0){}
Server::~Server();
Server &    Server::operator=(Server const cpy)
{
    if (this != &cpy)
    {
        _socket = cpy._socket;
        _bind = cpy._bind;
        _listen = cpy._listen;
    }
    return (*this);
}
Server::Server(Server const & cpy)
{
    _socket = cpy._socket;
    _bind = cpy._bind;
    _listen = cpy._listen;
}

int const & Server::getSocket() const
{
    return (_socket);
}
int const & Server::getListen() const
{
    return (_listen);
}
int const & Server::getBind() const
{
    return (_bind);
}
int Server::setSocket(int const domain, int const type, int protocol)
{
    _socket = socket(domain, type, protocol);
}
int Server::setBind(int const socketfd, const struct sockaddr *addr, socklen_t const addrlen)
{
    _bind = bind(socketfd, addr, addrlen);
}
int Server::setListen(int const socketfd, int const backlog)
{
    _listen = listen(socketfd, backlog);
}
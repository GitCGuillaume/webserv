#include "Server.hpp"

Server::Server() : _socket(0), _epfd(epoll_create(1))
{
    if (_epfd < 0)
        throw ServerException(strerror(errno));
}

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

static int epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	return (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev));
}

void    Server::createNewSocket(int port)
{
    int newSock = socket(AF_INET, SOCK_STREAM, 0);
    if (newSock < 0)
        throw ServerException(strerror(errno));
    struct sockaddr_in addr;
    bzero(&addr, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(newSock, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw ServerException(strerror(errno));
    if (listen(newSock, 5) < 0)
        throw ServerException(strerror(errno));
    if (epoll_ctl_add(_epfd, newSock, EPOLLIN | EPOLLOUT | EPOLLET) < 0)
        throw ServerException(strerror(errno));
}

void    Server::loop()
{
    struct epoll_event events[20];
    while (1)
    {
        epoll_wait(_epfd, events, 20, 5000);
    }
}

int const & Server::getSocket() const
{
    return (_socket);
}

void Server::setSocket(int const domain, int const type, int protocol)
{
    _socket = socket(domain, type, protocol);
}

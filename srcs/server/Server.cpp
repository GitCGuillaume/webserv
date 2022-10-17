#include "Server.hpp"

Server::Server() : _socket(0), _epfd(epoll_create(1))
{
    if (_epfd < 0)
        throw ServerException("epoll create constructor", strerror(errno));
}

Server::~Server()
{
    for (std::set<int>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
        close(*it);
}

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
        throw ServerException("socket createNewSocket", strerror(errno));
    struct sockaddr_in addr;
    bzero(&addr, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(newSock, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw ServerException("bind createNewSocket", strerror(errno));
    if (listen(newSock, 5) < 0)
        throw ServerException("listen createNewSocket", strerror(errno));
    if (epoll_ctl_add(_epfd, newSock, EPOLLIN | EPOLLOUT | EPOLLET) < 0)
        throw ServerException("epoll_ctl_add createNewSocket", strerror(errno));
    _sockets.insert(newSock);
}

void    Server::loop()
{
    char buf [6000];
    struct epoll_event events[20];
    int nb_fds;
    struct sockaddr_in cli_addr;
    socklen_t s_len (sizeof(cli_addr));
    char inet[16];
    while (1)
    {
        nb_fds = epoll_wait(_epfd, events, 20, 5000);
        for (int i = 0; i < nb_fds; ++i)
        {
            if (_sockets.find(events[i].data.fd) != _sockets.end())
            {
                int sockClient = accept(events[i].data.fd, (sockaddr *)&cli_addr, &s_len);
                inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr), inet, sizeof(cli_addr));
				std::cout << "[+] connected with " << inet << ": " << ntohs(cli_addr.sin_port) << std::endl;
                epoll_ctl_add(_epfd, sockClient, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
            }
            if (events[i].events & EPOLLIN)
            {
                bzero(buf, 6000);
                recv(events[i].data.fd, buf, 6000, 0);
                std::cout << buf;
            }
            if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				std::cout << "[+] connection closed" << std::endl;
				epoll_ctl(_epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
				close(events[i].data.fd);
			}   
        }
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

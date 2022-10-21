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

static int setnonblocking(int sockfd)
{
	if (fcntl(sockfd, F_SETFD, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) ==
	    -1) {
		return -1;
	}
	return 0;
}

void    Server::createNewSocket(int port)
{
    int opt = 1;
    int newSock = socket(AF_INET, SOCK_STREAM, 0);
    if (newSock < 0)
        throw ServerException("socket createNewSocket", strerror(errno));
    
    if (setsockopt(newSock, SOL_SOCKET,
            SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        throw ServerException("setsockopt createNewSocket", strerror(errno));

    if (setnonblocking(newSock) < 0)
        throw ServerException("setnonblocking createNewSocket", strerror(errno));

    struct sockaddr_in addr;
    bzero(&addr, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(newSock, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw ServerException("bind createNewSocket", strerror(errno));
    if (listen(newSock, 5) < 0)
        throw ServerException("listen createNewSocket", strerror(errno));
    if (epoll_ctl_add(_epfd, newSock, EPOLLIN) < 0)
        throw ServerException("epoll_ctl_add createNewSocket", strerror(errno));
    _sockets.insert(newSock);
}

void    Server::loop()
{
    epoll_event events[20];
    int nb_fds;
    struct sockaddr_in cli_addr;
    socklen_t s_len (sizeof(cli_addr));
    char inet[16];
    while (1)
    {
        nb_fds = epoll_wait(_epfd, events, 20, -1);
        for (int i = 0; i < nb_fds; ++i)
        {
            _curr_event = events[i];
            if (_sockets.find(_curr_event.data.fd) != _sockets.end())
            {
                int sockClient = accept(_curr_event.data.fd, (sockaddr *)&cli_addr, &s_len);
                if (setnonblocking(sockClient) < 0)
                    throw ServerException("setnonblocking createNewSocket", strerror(errno));
                inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr), inet, sizeof(cli_addr));
				std::cout << "[+] connected with " << inet << ": " << ntohs(cli_addr.sin_port) << std::endl;
                std::cout << sockClient << " client server " << _curr_event.data.fd << std::endl;
                epoll_ctl_add(_epfd, sockClient, EPOLLIN | EPOLLRDHUP | EPOLLHUP);
                _clients.insert(std::pair<int,Client>(sockClient, Client(ntohs(cli_addr.sin_port), sockClient)));
            }
           
            else if (_curr_event.events & EPOLLIN)
            {
                if (setnonblocking(_curr_event.data.fd) < 0)
                    throw ServerException("setnonblocking createNewSocket", strerror(errno));
                //std::cout << "here " << _curr_event.data.fd << std::endl;
                std::map<int, Client>::iterator it = _clients.find(_curr_event.data.fd);
                if (it != _clients.end())
                    it->second.epoll_in();
            }
             
            if (_curr_event.events & (EPOLLRDHUP | EPOLLHUP)) {
				std::cout << "[+] connection closed" << std::endl;
				epoll_ctl(_epfd, EPOLL_CTL_DEL, _curr_event.data.fd, NULL);
				close(_curr_event.data.fd);
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

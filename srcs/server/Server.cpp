#include "Server.hpp"

Server::Server(const char *conf) : _socket(0), _epfd(epoll_create(1)), _config(conf)
{
    if (_epfd < 0)
        throw ServerException("epoll create constructor", strerror(errno));
    s_general_header::init_map_ge_headers();
    s_request_header::init_map_re_headers();
    s_entity_header::init_map_en_headers();
    s_entity_header::init_map_mime();
    Response::init_map_method();
    Response::init_map_error();
    init_map_config();
}

Server::~Server()
{
    for (std::set<int>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
        close(*it);
}

Server &Server::operator=(Server const cpy)
{
    if (this != &cpy)
    {
        _socket = cpy._socket;
    }
    return (*this);
}
Server::Server(Server const &cpy) : _socket(cpy._socket), _config(cpy._config)
{
}

static int epoll_ctl_add(int epfd, int fd, uint32_t events)
{
    struct epoll_event ev;
    bzero(&ev.data, sizeof(ev.data));
    ev.events = events;
    ev.data.fd = fd;
    return (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev));
}

static int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFD, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
        return -1;
    return 0;
}

void Server::createNewSocket(uint16_t port)
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

void Server::init_map_config(void)
{
    for (Config::vect_serv::const_iterator it = _config.getServers().begin(); it != _config.getServers().end(); ++it)
    {
        Config::ptr_server ref(&*it);
        for (Config::vect_listens::const_iterator it2 = it->listens.begin(); it2 != it->listens.end(); ++it2)
        {
            if (_map_config.find(*it2) == _map_config.end())
            {
                _map_config.insert(std::make_pair(*it2, &*it));
                createNewSocket(it2->second);
            }
        }
    }
}

void Server::loop()
{
    epoll_event events[MAX_EVENTS];
    int nb_fds;
    struct sockaddr_in cli_addr;
    socklen_t s_len(sizeof(cli_addr));
    std::pair<std::string, uint16_t> host;
    while (1)
    {
        nb_fds = epoll_wait(_epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nb_fds; ++i)
        {
            _curr_event = events[i];
            if (_sockets.find(_curr_event.data.fd) != _sockets.end())
            {
                
                int sockClient = accept(_curr_event.data.fd, (sockaddr *)&cli_addr, &s_len);
                if (sockClient < 0)
                {
                    throw ServerException("accept",strerror(errno));
                }
                if (setnonblocking(sockClient) < 0)
                    throw ServerException("setnonblocking createNewSocket", strerror(errno));
                host.first = inet_ntoa(cli_addr.sin_addr);
                getsockname(sockClient, reinterpret_cast<sockaddr *>(&cli_addr), &s_len);
                host.second = ntohs(cli_addr.sin_port);
                if (_map_config.find(host) != _map_config.end())
                {
                    std::cout << "[+] connected with " << host.first << ": " << host.second << std::endl;
                    std::cout << sockClient << " client server " << _curr_event.data.fd << std::endl;
                    epoll_ctl_add(_epfd, sockClient, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP);
                    _clients.insert(std::pair<int, Client>(sockClient, Client(host, sockClient, _map_config[host])));
                }
                else
                {
                }
            }

            else if (_curr_event.events & EPOLLIN)
            {
                // if (setnonblocking(_curr_event.data.fd) < 0)
                //     throw ServerException("setnonblocking createNewSocket", strerror(errno));
                std::map<int, Client>::iterator it = _clients.find(_curr_event.data.fd);
                if (it != _clients.end())
                    it->second.epoll_in();
            }
            else if (_curr_event.events & EPOLLOUT)
            {
                // if (setnonblocking(_curr_event.data.fd) < 0)
                //     throw ServerException("setnonblocking createNewSocket", strerror(errno));
                std::map<int, Client>::iterator it = _clients.find(_curr_event.data.fd);
                if (it != _clients.end())
                    it->second.epoll_out();
            }

            if (_curr_event.events & (EPOLLRDHUP | EPOLLHUP))
            {
                std::cout << "[+] connection closed" << std::endl;
                epoll_ctl(_epfd, EPOLL_CTL_DEL, _curr_event.data.fd, NULL);
                close(_curr_event.data.fd);
                _clients.erase(_curr_event.data.fd);
            }
        }
    }
}

int const &Server::getSocket() const
{
    return (_socket);
}

void Server::setSocket(int const domain, int const type, int protocol)
{
    _socket = socket(domain, type, protocol);
}

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <set>
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "Config.hpp"

#define MAX_EVENTS 10000


class Server
{

public:
    typedef struct epoll_event epoll_event;

    Server(const char *conf);
    ~Server();
    Server &operator=(Server const cpy);
    Server(Server const &cpy);

    void createNewSocket(uint16_t port);
    void init_map_config(void);
    void poll_in();
    void loop(void);
    int const &getSocket() const;
    void setSocket(int const domain, int const type, int protocol);
private:
    std::set<int> _sockets;
    int _socket;
    std::map<int, Client> _clients;
    int _epfd;
    epoll_event _curr_event;
    Config _config;
    std::map<const std::pair<std::string, uint16_t>, Config::ptr_server> _map_config; 

    class ServerException: public std::exception
    {
    private:
        std::string m_error;

    public:
        ServerException(const std::string &fun, const std::string &error)
            : m_error(fun + ": " + error)
        {
        }
        ~ServerException() throw(){}
        const char *what() const throw() { return (m_error.c_str());}
    };
};

#endif
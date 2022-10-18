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
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
# define BUFFER_SIZE 5120


class Server
{

public:
    typedef struct epoll_event epoll_event;

    Server();
    ~Server();
    Server &operator=(Server const cpy);
    Server(Server const &cpy);

    void createNewSocket(int port);
    void poll_in();
    void loop(void);

    int const &getSocket() const;
    void setSocket(int const domain, int const type, int protocol);

private:
    std::set<int> _sockets;
    int _socket;
    std::map<int, Client> _clients;
    std::map<int, Request> _requests;
    int _epfd;
    epoll_event _curr_event;

    class ServerException
    {
    private:
        std::string m_error;

    public:
        ServerException(const std::string &fun, const std::string &error)
            : m_error(fun + ": " + error)
        {
        }

        const std::string &getError() const { return m_error; }
    };
};

#endif
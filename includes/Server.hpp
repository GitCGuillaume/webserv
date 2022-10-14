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
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class Server
{
private:
    std::set<int> _sockets;
    int _socket;
    std::map<int, Client> _clients;
    int _epfd;

public:
    Server();
    ~Server();
    Server &operator=(Server const cpy);
    Server(Server const &cpy);

    void createNewSocket(int port);
    void loop(void);

    int const &getSocket() const;
    void setSocket(int const domain, int const type, int protocol);

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
#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
    private:
        int _socket;
        int _listen;
        int _bind;
    
    public:
        Server();
        ~Server();
        Server &    operator=(Server const cpy);
        Server(Server const & cpy);
        int const & getSocket() const;
        int const & getListen() const;
        int const & getBind() const;
        int setSocket(int const domain, int const type, int protocol);
        int setBind(int const socketfd, const struct sockaddr *addr, socklen_t const addrlen);
        int setListen(int const socketfd, int const backlog);
};

#endif
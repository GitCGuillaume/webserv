#ifndef SERVER_INPUT_HPP
#define SERVER_INPUT_HPP
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include "Request.hpp"

#define BUFFER_SIZE 1

class Client;
class ServerInput
{
private:
    Request _req;
    const Client &_handler;
    int _socket;
    size_t _pos_end_header;
    class ServerInputException : public std::exception
    {
    private:
        std::string m_error;

    public:
        ServerInputException(const std::string &fun, const std::string &error)
            : m_error(fun + ": " + error)
        {
        }
        ~ServerInputException() throw() {}
        const char *what() const throw() { return (m_error.c_str()); }
    };

public:
    ServerInput(const Client &handler, int sock);
    ServerInput(const ServerInput &src);
    ~ServerInput();

    void readData();
    void reset();

    const Request &getReq(void) const;
};

#endif
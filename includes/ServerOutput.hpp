#ifndef SERVER_OUTPUT_HPP
#define SERVER_OUTPUT_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include "Response.hpp"

class Response;
class ServerOutput
{
private:
    class ServerOutputException : public std::exception
    {
    private:
        std::string m_error;

    public:
        ServerOutputException(const std::string &fun, const std::string &error)
            : m_error(fun + ": " + error)
        {
        }
        ~ServerOutputException() throw() {}
        const char *what() const throw() { return (m_error.c_str()); }
    };
    int _socket;

public:
    ServerOutput(int socket);
    ~ServerOutput();

    void sendResponse(const Response &response);
};

#endif
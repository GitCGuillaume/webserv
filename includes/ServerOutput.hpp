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
    int _socket;

public:
    ServerOutput(int socket);
    ~ServerOutput();

    void sendResponse(const Response &response);
};

#endif
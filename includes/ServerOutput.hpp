#ifndef SERVER_OUTPUT_HPP
#define SERVER_OUTPUT_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

class ServerOutput
{
private:
    int _socket;

public:
    ServerOutput(int socket);
    ~ServerOutput();

    void sendResponse(void);
};

#endif
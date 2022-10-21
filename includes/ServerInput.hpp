#ifndef SERVER_INPUT_HPP
#define SERVER_INPUT_HPP
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "Request.hpp"

#define BUFFER_SIZE 1

class Client;
class ServerInput
{
private:
    Request _req;
    Client &_handler;
    int _socket;
public:
    ServerInput(Client &handler, int sock);
    ServerInput(const ServerInput &src);
    ~ServerInput();

    void readData();
};



#endif
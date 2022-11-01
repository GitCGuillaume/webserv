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
    Client &_handler;
    int _socket;
    size_t _pos_end_header;
public:
    ServerInput(Client &handler, int sock);
    ServerInput(const ServerInput &src);
    ~ServerInput();

    void readData();
    void reset();

    const  Request &getReq(void);
};



#endif
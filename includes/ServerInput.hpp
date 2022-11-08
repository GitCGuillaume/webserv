#ifndef SERVER_INPUT_HPP
#define SERVER_INPUT_HPP
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include "Request.hpp"

#define BUFFER_SIZE 1

class ServerInput
{
private:
    int _socket;
    size_t _pos_end_header;
    Request _req;

public:
    typedef std::pair<std::string, uint16_t> host_type;
    ServerInput(host_type ip, int sock);
    ServerInput(const ServerInput &src);
    ~ServerInput();

    void readData();
    void reset();

    const Request &getReq(void);
};

#endif
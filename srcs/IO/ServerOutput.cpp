#include "ServerOutput.hpp"

ServerOutput::ServerOutput(int sock): _socket(sock)
{
}

ServerOutput::~ServerOutput()
{
}

void ServerOutput::sendResponse(void)
{
    std::string response(
        "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, world!"
    );
    std::cout << response;
    send(_socket, response.c_str(),response.size() + 1, 0);
}





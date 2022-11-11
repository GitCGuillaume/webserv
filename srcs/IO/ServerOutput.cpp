#include "ServerOutput.hpp"

ServerOutput::ServerOutput(int sock) : _socket(sock)
{
}

ServerOutput::~ServerOutput()
{
}

void ServerOutput::sendResponse(const Response &response)
{
    std::string resp = response.seralize();
    std::cout << response;

    if (send(_socket, resp.c_str(), resp.size() + 1, 0) == -1)
        throw ServerOutputException("sendResponse send", strerror(errno));
}

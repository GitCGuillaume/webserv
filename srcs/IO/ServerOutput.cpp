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
    ssize_t ret = send(_socket, resp.c_str(), resp.size() + 1, 0);
    if (ret == -1)
        throw ServerOutputException("sendResponse", strerror(errno));
    else if (static_cast<size_t>(ret) < resp.size() + 1)
        throw ServerOutputException("sendResponse", "send");
}

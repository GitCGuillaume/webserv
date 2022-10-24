#include "ServerInput.hpp"


ServerInput::ServerInput(Client &handler, int sock): _handler(handler), _socket(sock)
{

}

ServerInput::ServerInput(const ServerInput &src): _handler(src._handler), _socket(src._socket)
{
}


ServerInput::~ServerInput()
{
}

void ServerInput::readData()
{
    static size_t len = 0;
    char buf [BUFFER_SIZE + 1];
    ssize_t n;

    n = recv(_socket, buf, BUFFER_SIZE, 0);
    buf[n] = '\0';
    _req += buf;
    if (len >= 3)
        len -= 3;
    if (_req.getReq().rfind("\r\n\r\n", len) != std::string::npos)
    {
        _req.parse();
        std::cout << _req << std::endl;
        len = 0;
    }
    len = _req.getReq().size();
}

void ServerInput::reset()
{
    _req.reset();
}

const  Request &ServerInput::getReq(void)
{
    return (_req);
}

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
    // static size_t len = 0;
    // static bool requestLine;
    // char buf [BUFFER_SIZE + 1];
    // ssize_t n;

    // n = recv(_socket, buf, BUFFER_SIZE, 0);
    // buf[n] = '\0';
    // _req += buf;
    // if (requestLine && _req.getReq().find("\r\n\r\n", len + 2) != std::string::npos)
    //     //|| _req.getReq().find("\n\n") != std::string::npos) // \r\n (CRLF) is always ending of http headers
    // {
    //     // std::cout << "-----headers----" << len << "\n";
    //     // std::cout << _req.getReq().substr(len + 2);
    //     //len = 0;
    // }
    // else if (!requestLine && (len = _req.getReq().find("\r\n")) != std::string::npos)
    // {
    //     std::cout << "-------Request line-----------\n";
    //     std::cout << _req << std::endl;
    //     requestLine = true;
    // }
    //std::cout << _req;
    static size_t len = 0;
    char buf [BUFFER_SIZE + 1];
    ssize_t n;

    n = recv(_socket, buf, BUFFER_SIZE, 0);
    buf[n] = '\0';
    _req += buf;
    if ((len = _req.getReq().find("\r\n\r\n")) != std::string::npos)
    {
        std::cout << _req << std::endl;
    }
}

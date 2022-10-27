#include "ServerInput.hpp"

ServerInput::ServerInput(Client &handler, int sock) : _handler(handler), _socket(sock)
{
}

ServerInput::ServerInput(const ServerInput &src) : _handler(src._handler), _socket(src._socket)
{
}

ServerInput::~ServerInput()
{
}

void ServerInput::readData()
{
    static size_t len = 0;
    static size_t end_req = 0;
    char buf[BUFFER_SIZE + 1];
    ssize_t n;
    int found = 0;
    n = recv(_socket, buf, BUFFER_SIZE, 0);
    
    _req.append_data(buf, n);
    if (_req.getHeader().content_length < 0)
    {
        if (len >= 3)
            len -= 3;
        if ((end_req = _req.getReq().find("\r\n\r\n", len)) != std::string::npos)
        {
            end_req += 4;
            _req.parse();
            if (_req.is_ready())
            {
                std::cout << "is ready" << std::endl;
                len = 0;
                return;
            }
        }
    }
    len = _req.size();
    // std::cout << "length " << _req.getHeader().content_length << std::endl;
    // std::cout << "len " << len << " end req " << end_req << std::endl;
    if (_req.getHeader().content_length >= 0 && static_cast<size_t>(_req.getHeader().content_length) <= (len - end_req))
    {
        len = 0;
        _req.parse_body(end_req);
    }
}

void ServerInput::reset()
{
    _req.reset();
}

const Request &ServerInput::getReq(void)
{
    return (_req);
}

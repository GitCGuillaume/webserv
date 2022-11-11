#include "ServerInput.hpp"
#include "Client.hpp"

ServerInput::ServerInput(const Client &handler, int sock) : _req(handler), _handler(handler), _socket(sock), _pos_end_header(0)
{
}

ServerInput::ServerInput(const ServerInput &src) : _req(src._req), _handler(src._handler), _socket(src._socket), _pos_end_header(src._pos_end_header)
{
}

ServerInput::~ServerInput()
{
}

void ServerInput::readData()
{
    size_t readBytes = _req.size();
    if (readBytes == 0)
        _req.set_time();
    _req.set_timeout();
    if (_req.is_timeout())
    {
        _req.setReady();
        return;
    }
    char buf[BUFFER_SIZE + 1];
    ssize_t n;
    int found = 0;
    n = recv(_socket, buf, BUFFER_SIZE, 0);
    if (n == -1)
        throw ServerInputException("readData recv", strerror(errno));
    _req.append_data(buf, n);
    if (_req.getEntityHeader().content_length.empty())
    {
        size_t tmp = readBytes;
        if (tmp >= 3)
            tmp -= 3;
        if ((_pos_end_header = _req.getReq().find("\r\n\r\n", tmp)) != std::string::npos)
        {
            _pos_end_header += 4;
            _req.parse();
            if (_req.is_ready())
            {
                readBytes = 0;
                _pos_end_header = 0;
                return;
            }
        }
    }
    readBytes += n;
    if (!_req.getEntityHeader().content_length.empty() && _req.getContentLength() <= (readBytes - _pos_end_header))
    {
        _req.parse_body(_pos_end_header);
        _pos_end_header = 0;
        readBytes = 0;
    }
}

void ServerInput::reset()
{
    _req.reset();
}

const Request &ServerInput::getReq(void) const
{
    return (_req);
}

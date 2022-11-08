#include "ServerInput.hpp"

ServerInput::ServerInput(host_type ip, int sock) : _socket(sock), _pos_end_header(0), _req(ip)
{
}

ServerInput::ServerInput(const ServerInput &src) : _socket(src._socket), _pos_end_header(src._pos_end_header), _req(src._req)
{
}

ServerInput::~ServerInput()
{
}

void ServerInput::readData()
{
    size_t readBytes = _req.size();
    char buf[BUFFER_SIZE + 1];
    ssize_t n;
    int found = 0;
    n = recv(_socket, buf, BUFFER_SIZE, 0);

    if (n == -1)
    {
        perror(0);
        exit(1);
    }
    
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
                std::cout << "is ready" << std::endl;
                readBytes = 0;
                _pos_end_header = 0;
                return;
            }
        }
    }
    readBytes += n;
    if (!_req.getEntityHeader().content_length.empty() && _req.getContentLength() <= (readBytes - _pos_end_header))
    {
        //std::cout << "LA" << std::endl;
        _req.parse_body(_pos_end_header);
        _pos_end_header = 0;
        readBytes = 0;
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

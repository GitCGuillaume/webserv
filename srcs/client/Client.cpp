#include "Client.hpp"

Client::Client(uint16_t port, int socket): _port(port), _si(*this, socket), _so(socket)
{
}

Client::Client(const Client &src): _port(src._port), _si(src._si), _so(src._so)
{
}

Client &Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		_port = rhs._port;
	}
	return (*this);
}
void    Client::epoll_in(void)
{
	_si.readData();
}

void    Client::epoll_out(void)
{
	if (_si.getReq().is_ready())
	{
		std::cout << "Ready to send ..." << std::endl;
		_so.sendResponse();
		_si.reset();
	}
}

Client::~Client()
{

}

int Client::getPort(void) const
{
	return (_port);
}

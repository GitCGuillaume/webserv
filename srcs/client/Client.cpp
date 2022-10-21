#include "Client.hpp"

Client::Client(uint16_t port, int socket): _port(port), _si(*this, socket)
{
}

Client::Client(const Client &src): _port(src._port), _si(src._si)
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

Client::~Client()
{

}

int Client::getPort(void) const
{
	return (_port);
}

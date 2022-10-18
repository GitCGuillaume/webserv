#include "Client.hpp"

Client::Client(uint16_t port, int socket): _port(port), _socket(socket)
{
}

Client::Client(const Client &src): _port(src._port), _socket(src._socket)
{
}

Client &Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		_socket = rhs._socket;
		_port = rhs._port;
	}
	return (*this);
}
void    Client::sendResponse(void)
{

}

Client::~Client()
{

}

int Client::getPort(void) const
{
	return (_port);
}

int Client::getSocket(void) const
{
	return (_socket);
}
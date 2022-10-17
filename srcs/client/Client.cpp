#include "Client.hpp"

Client::Client(int port): _port(port)
{
}

Client::Client(const Client &src): _port(src._port)
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
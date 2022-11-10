#include "Client.hpp"

Client::Client(host_type ip, int socket, Config::ptr_server conf): _ip(ip), _conf(conf), _si(*this, socket), _so(socket)
{
	if (_conf)
		std::cout << "Client " << _conf->server_name << std::endl;
}

Client::Client(const Client &src): _ip(src._ip), _conf(src._conf), _si(src._si), _so(src._so)
{
}

Client &Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		_ip = rhs._ip;
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
		std:: cout << "-------request----------" << std::endl;
		std::cout << "Ready to send ..." << std::endl;
		std::cout << _si.getReq();
		Response rep (_si.getReq(), _conf);
		_so.sendResponse(rep);
		_si.reset();
	}
}

Client::~Client()
{

}

const Client::host_type &Client::getIp(void) const
{
	return (_ip);
}

Config::ptr_server Client::get_conf() const
{
	return _conf;
}

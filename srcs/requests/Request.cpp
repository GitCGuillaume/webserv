#include "Request.hpp"

Request::Request()
{

}

Request::Request(const std::string  &req): _req(req)
{
}

Request::Request(const Request &src): _req(src._req)
{
}

Request &Request::operator=(const Request &rhs)
{
	if (&rhs != this)
	{
		_req = rhs._req;
	}
	return (*this);
}

Request &Request::operator=(const std::string &rhs)
{
	_req = rhs;
	return (*this);
}


Request &Request::operator+=(const std::string &rhs)
{
	_req += rhs;
	return (*this);
}

std::ostream &operator<<(std::ostream &os, const Request &rhs)
{
	os << rhs._req;
	return (os);
}


Request::~Request()
{
}

const std::string &Request::getReq(void) const
{
	return (_req);
}

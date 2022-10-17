#include "webserv.hpp"
#include "Server.hpp"

int main()
{
	Server s;

	s.createNewSocket(9000);
	s.createNewSocket(9001);
	s.createNewSocket(9002);
	s.loop();
	return (0);
}
#include "webserv.hpp"
#include "Server.hpp"

int main()
{
	Server s;

	s.createNewSocket(8000);
	// s.createNewSocket(9001);
	// s.createNewSocket(9002);
	//s.createNewSocket(80);
	s.loop();
	return (0);
}
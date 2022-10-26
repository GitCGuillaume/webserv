#include "webserv.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	Server s;
    Config c(argv[1]);

	try
	{
		s.createNewSocket(8003);
		/* code */
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	// s.createNewSocket(9001);
	// s.createNewSocket(9002);
	//s.createNewSocket(80);
	s.loop();
	return (0);
}
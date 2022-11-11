#include "webserv.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc <= 1 || argc > 2)
	{
		std::cerr << "Need 1 arg" << std::endl;
		return (1);
	}
	Config c(argv[1]);

	try
	{
		Server s(argv[1]);
		s.loop();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
#include "webserv.hpp"
#include "Server.hpp"

void load_directory_autoindex(std::string &ret_html);
int main(int argc, char **argv)
{
	Config c(argv[1]);

	try
	{
		Server s(argv[1]);
		s.loop();
		/* code */
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	// s.createNewSocket(9001);
	// s.createNewSocket(9002);
	// s.createNewSocket(80);

	return (0);
}
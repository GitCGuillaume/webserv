#include "webserv.hpp"
#include "Server.hpp"

void load_directory_autoindex(std::string &ret_html);
int main(int argc, char **argv)
{
	
	if (argc > 2)
	{
		std::cerr << "Too many arguments. Usage: ./webserv [conf]" << std::endl;
		return (1);
	}
	std::string conf_name;
	if (argc == 1)
		conf_name = "tester/conf/default";
	else
		conf_name = argv[1];

	try
	{
		Server s(conf_name.c_str());
		s.loop();
		/* code */
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
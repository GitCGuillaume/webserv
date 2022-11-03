// #include "webserv.hpp"
// #include "Server.hpp"
#include "Config.hpp"


void load_directory_autoindex(std::string &ret_html);
int main(int argc, char **argv)
{
<<<<<<< HEAD
	// Server s;
	try
	{
	Config c(argv[1]);
	} catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}	

	// try
	// {
	// 	s.createNewSocket(8003);
	// 	/* code */
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	
	// s.createNewSocket(9001);
	// s.createNewSocket(9002);
	//s.createNewSocket(80);
	// s.loop();
=======
	Server s;
	Config c(argv[1]);

	try
	{
		s.createNewSocket(8003);
		/* code */
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	// s.createNewSocket(9001);
	// s.createNewSocket(9002);
	// s.createNewSocket(80);
	s_general_header::init_map_ge_headers();
	s_request_header::init_map_re_headers();
	s_entity_header::init_map_en_headers();
	Response::init_map_method();
	s.loop();
>>>>>>> origin/greg
	return (0);
}
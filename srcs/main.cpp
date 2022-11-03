#include "webserv.hpp"
#include "Server.hpp"

void load_directory_autoindex(std::string &ret_html);
int main(int argc, char **argv)
{
	Config c(argv[1]);

	try
	{
		Server s(argv[1]);
		s_general_header::init_map_ge_headers();
		s_request_header::init_map_re_headers();
		s_entity_header::init_map_en_headers();
		Response::init_map_method();
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
#include "webserv.hpp"
#include "Server.hpp"

int main()
{
	Server s;
	std::list<char *>	&lst = load_directory_autoindex();
	open_stream_autoindex(lst);
	/*s.createNewSocket(9000);
	s.createNewSocket(9001);
	s.createNewSocket(9002);
	s.loop();
	*/
	return (0);
}
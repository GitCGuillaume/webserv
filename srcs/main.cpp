#include "webserv.hpp"
#include "Server.hpp"
#include "Cgi.hpp"
#include <sstream>
/*
static void	seek_pathinfo(char **envp, std::string & path_info)
{
	if (!envp || !*envp)
		return ;
	std::string	cmp_str("PATH_INFO");
	std::string	seek_char(envp[0]);
	unsigned int	i = 0;
	unsigned int	j = 0;
	int found = 1;

	while (envp[i] && found != 0)
	{
		seek_char = envp[i];
		while (envp[i][j] && envp[i][j] != '=')
			++j;
		found = cmp_str.compare(seek_char.substr(0, j));
		if (found == 0)
			break ;
		++i;
		j = 0;
	}
	if (found == 0)
		path_info = seek_char.substr(j + 1, seek_char.length());
}*/

int main()
{
	/* POST */
	/*Cgi cgi("CONTENT_LENGTH=3","CONTENT_TYPE=application/x-www-form-urlencoded","GATEWAY_INTERFACE=CGI/1.1",
		"PATH_INFO=tester/www/website/cgi-bin/YoupiBanane/youpi.bla","",
		"QUERY_STRING=val1=d","REMOTE_ADDR=127.0.0.1","REMOTE_HOST=127.0.0.1","REQUEST_METHOD=POST",
		"SCRIPT_NAME=","SERVER_NAME=localhost","SERVER_PORT=80","SERVER_PROTOCOL=HTTP/1.1");
	*/
	Cgi cgi("CONTENT_LENGTH=3","CONTENT_TYPE=application/x-www-form-urlencoded","GATEWAY_INTERFACE=CGI/1.1",
		"PATH_INFO=/website/cgi-bin/post.php","PATH_TRANSLATED=/home/gchopin/Documents/webserv/tester/www/website/cgi-bin/post.php",
		"QUERY_STRING=","REMOTE_ADDR=127.0.0.1","REMOTE_HOST=127.0.0.1","REQUEST_METHOD=POST",
		"SCRIPT_NAME=","SERVER_NAME=localhost","SERVER_PORT=80","SERVER_PROTOCOL=HTTP/1.1");
	
	/* GET */
	/*Cgi cgi("CONTENT_LENGTH=","CONTENT_TYPE=application/x-www-form-urlencoded","GATEWAY_INTERFACE=CGI/1.1",
		"PATH_INFO=/website/cgi-bin/get.php","PATH_TRANSLATED=/home/gchopin/Documents/webserv/tester/www/website/cgi-bin/get.php",
		"QUERY_STRING=test1=1&test2=éé&test3=ff","REMOTE_ADDR=127.0.0.1","REMOTE_HOST=127.0.0.1","REQUEST_METHOD=GET",
		"SCRIPT_NAME=","SERVER_NAME=localhost","SERVER_PORT=80","SERVER_PROTOCOL=HTTP/1.1");
	*/
	/* DELETE */
	/*Cgi cgi("CONTENT_LENGTH=","CONTENT_TYPE=application/x-www-form-urlencoded","GATEWAY_INTERFACE=CGI/1.1",
		"PATH_INFO=/website/cgi-bin/delete.php","PATH_TRANSLATED=/home/gchopin/Documents/webserv/tester/www/website/cgi-bin/delete.php",
		"QUERY_STRING=","REMOTE_ADDR=127.0.0.1","REMOTE_HOST=127.0.0.1","REQUEST_METHOD=DELETE",
		"SCRIPT_NAME=","SERVER_NAME=localhost","SERVER_PORT=80","SERVER_PROTOCOL=HTTP/1.1");
	*/
	/* UPLOAD */
	
	cgi.start();
	//Server s;
	//std::string path_info;
	//seek_pathinfo(envp, path_info);
	/*try{
		std::stringstream ret_html(std::ios_base::ate | std::ios_base::in | std::ios_base::out);
		load_directory_autoindex(ret_html);
		std::cout<<ret_html.str();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}*/
	/*s.createNewSocket(9000);
	s.createNewSocket(9001);
	s.createNewSocket(9002);
	s.loop();
	*/
	return (0);
}
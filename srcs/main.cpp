#include "webserv.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>  // <cstring> en C++

int main()
{
	int	ft_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in	serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9990);
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	if (ft_socket < 0)
		std::cerr << "Error socket" << std::endl;
	else
		std::cout << "Socket OK" << std::endl;
	int	ft_socket2 = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in	serv_addr2;
	serv_addr2.sin_family = AF_INET;
	serv_addr2.sin_port = htons(9991);
	serv_addr2.sin_addr.s_addr = htons(INADDR_ANY);
	if (ft_socket2 < 0)
		std::cerr << "Error socket" << std::endl;
	else
		std::cout << "Socket OK" << std::endl;

	int ft_listen = listen(ft_socket, 5);
	if (ft_listen < 0)
		std::cerr << "Listen failed code : " << ft_listen
		<< strerror(errno) << std::endl;
	else
		std::cout << "Listen OK" << std::endl;
	int ft_listen2 = listen(ft_socket2, 5);
	if (ft_listen2 < 0)
		std::cerr << "Listen2 failed code : " << ft_listen2
		<< strerror(errno) << std::endl;
	else
		std::cout << "Listen2 OK" << std::endl;
	int ft_connect = connect(ft_socket, (sockaddr*)&serv_addr, sizeof(serv_addr));
	if (ft_connect < 0)
		std::cerr << "Error connect on socket failed code : "
			<< strerror(errno) << ft_connect << "errno : " << errno << std::endl;
	else
		std::cout << "Connect OK" << std::endl;
	int ft_connect2 = connect(ft_socket2, (sockaddr*)&serv_addr2, sizeof(serv_addr2));
	if (ft_connect2 < 0)
		std::cerr << "Error connect on socket failed code : "
			<< strerror(errno) << ft_connect2 << "errno : " << errno << std::endl;
	else
		std::cout << "Connect OK" << std::endl;
	while (1){}
	return (0);
}
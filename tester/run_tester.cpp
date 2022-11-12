#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/epoll.h>
/* man 7 ip */

typedef struct s_message
{

} t_message;

void ft_close(int fd_socket)
{
	if (close(fd_socket) < 0)
	{
		std::cerr << "Couldn't close properly" << std::endl;
		exit(1);
	}
}

int connectSocket(int ft_socket, uint16_t port)
{
	struct sockaddr_in serv_addr;
	int ft_connect = 0;

	std::memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (serv_addr.sin_addr.s_addr == INADDR_NONE)
	{
		ft_close(ft_socket);
		std::cerr << "Address invalid." << std::endl;
		return (1);
	}
	ft_connect = connect(ft_socket, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(serv_addr));
	if (ft_connect < 0)
	{
		ft_close(ft_socket);
		std::cerr << "Couldn't connect to socket." << std::endl;
		return (1);
	}
	else
		std::cout << "Connected to socket with "
				  << port << " port." << std::endl;
	return (0);
}

static int epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	return (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev));
}

int main(int argc, char **argv)
{
	ssize_t msg = 0;
	uint16_t port = 0;
	int ft_connect = 0;
	int ft_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (ft_socket < 0)
	{
		std::cerr << "Error socket : " << ft_socket << std::endl;
		return (1);
	}
	if (argc < 2)
	{
		ft_close(ft_socket);
		std::cerr << "please enter a port." << std::endl;
		return (1);
	}
	std::istringstream(argv[1]) >> port;
	ft_connect = connectSocket(ft_socket, port);
	if (ft_connect == 1)
		return (1);
	std::cout << "Sending message..." << std::endl;
	std::string message("GET /phpver/ HTTP/1.1\r\n\
Host: localhost:79\r\n\r\n");
	msg = send(ft_socket, message.c_str(), message.size() + 1, 0);
	sleep(1);
	if (msg < 0)
		std::cerr << "Couldn't send message." << std::endl;
	char buffer[1000000];

	msg = recv(ft_socket, buffer, sizeof(char) * 1000000, 0);
	if (msg < 0)
	{
		ft_close(ft_socket);
		std::cerr << "Couldn't receive message." << std::endl;
		return (1);
	}
	std::cout << buffer << std::endl;
	std::cout << std::endl;
	ft_close(ft_socket);
	return (0);
}
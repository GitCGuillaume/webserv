#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/epoll.h>
#include <fstream>
#include <iterator>
/* man 7 ip */

typedef struct s_message
{
	
}	t_message;

void	ft_close(int fd_socket)
{
	if (close(fd_socket) < 0)
	{
		std::cerr << "Couldn't close properly" << std::endl;
		exit(1);
	}
}

int	connectSocket(int ft_socket, uint16_t port)
{
	struct sockaddr_in	serv_addr;
	int	ft_connect = 0;

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
/*
static int epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	return (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev));
}
*/

void	upload_single_file(std::string & header, std::string link)
{
	std::ifstream file(link, std::ifstream::in | std::ifstream::binary);
	std::istreambuf_iterator<char>	it(file), ite;
	std::string	buf(it, ite);

	header = "POST /website/cgi-bin/upload_file.php HTTP/1.1\r\n"
	"Host: 127.0.0.1\r\n"
	"Content-Type: multipart/form-data; boundary=myboundary\r\n"
	"Content-Length: 5685\r\n"
	"Connection: keep-alive\r\n"
	"\r\n"//length start after \n
	"--myboundary\r\n";//14
	header.insert(header.size(),
		"Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"horizontal-logo-monochromatic-white.jpg\"\r\n");//107
	header.insert(header.size(), "Content-Type: image/jpeg\r\n\r\n");//28
	header.insert(header.size(), buf);//5518
	header.insert(header.size(), "\r\n--myboundary\r\n\r\n");//18
}

void	upload_multiple_file(std::string & header)
{
	std::ifstream file("www/website/pictures/42_Logo.svg", std::ifstream::in | std::ifstream::binary);
	std::ifstream file2("test.txt", std::ifstream::in | std::ifstream::binary);
	std::istreambuf_iterator<char>	it(file), ite;
	std::istreambuf_iterator<char>	it2(file2), ite2;
	std::string	buf(it, ite);
	std::string	buf2(it2, ite2);

	header = "POST /website/cgi-bin/upload_file.php HTTP/1.1\r\n"
	"Host: 127.0.0.1\r\n"
	"Content-Type: multipart/form-data; boundary=myboundary\r\n"
	"Content-Length: 5192\r\n"
	"Connection: keep-alive\r\n"
	"\r\n"//length start after \n
	"--myboundary\r\n";//14
	header.insert(header.size(),
		"Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"logo_42.jpg\"\r\n");//79
	header.insert(header.size(), "Content-Type: image/svg+xml\r\n\r\n");//31
	header.insert(header.size(), buf);//4928
	header.insert(header.size(), "\r\n--myboundary\r\n");//16
	header.insert(header.size(),
		"Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test.txt\"\r\n");//76
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");//28
	header.insert(header.size(), buf2);//2
	header.insert(header.size(), "\r\n--myboundary\r\n\r\n");//18
}

void	upload_single_all(std::string link, int ft_socket)
{
	char buffer2[50000];
	std::string	header;
	ssize_t	msg = 0;

	upload_single_file(header, link);
	msg = send(ft_socket, header.c_str(), header.size(), 0);
	sleep(1);
	if (msg < 0)
		std::cerr << "Couldn't send message." << std::endl;
	msg = recv(ft_socket, buffer2, sizeof(buffer2), 0);
	if (msg < 0)
	{
		ft_close(ft_socket);
		std::cerr << "Couldn't receive message." << std::endl;
		return ;
	}
	std::cout << buffer2 << std::endl;
}

int	main(int argc, char **argv)
{
	ssize_t	msg = 0;
	uint16_t 	port = 0;
	int	ft_connect = 0;
	int	ft_socket = socket(AF_INET, SOCK_STREAM, 0);

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
	std::string	header;
	
	upload_multiple_file(header);
	msg = send(ft_socket, header.c_str(), header.size(), 0);
	sleep(1);
	if (msg < 0)
		std::cerr << "Couldn't send message." << std::endl;
	char buffer[50000];
	msg = recv(ft_socket, buffer, sizeof(buffer), 0);
	if (msg < 0)
	{
		ft_close(ft_socket);
		std::cerr << "Couldn't receive message." << std::endl;
		return (1);
	}
	std::cout << buffer << std::endl;
	upload_single_all("www/website/pictures/horizontal-logo-monochromatic-white.jpg", ft_socket);
	ft_close(ft_socket);
	return (0);
}
/*
int	main(int argc, char **argv)
{
	ssize_t	msg = 0;
	uint16_t 	port = 0;
	// int	ft_socket = socket(AF_INET, SOCK_STREAM, 0);
	// int	ft_connect = 0;
	
	// if (ft_socket < 0)
	// {
	// 	std::cerr << "Error socket : " << ft_socket << std::endl;
	// 	return (1);
	// }
	// if (argc < 2)
	// {
	// 	ft_close(ft_socket);
	// 	std::cerr << "please enter a port." << std::endl;
	// 	return (1);
	// }
	// std::istringstream(argv[1]) >> port;
	// ft_connect = connectSocket(ft_socket, port);
	// if (ft_connect == 1)
	// 	return (1);
	// std::cout << "Sending message..." << std::endl;
	*/
	/*msg = send(ft_socket, "", sizeof("Test message"), 0);
	if (msg < 0)
		std::cerr << "Couldn't send message." << std::endl;
	*/
	/*
	int epfd = epoll_create(1);
	char buffer[50000];
	struct sockaddr_in cli_addr;
	int newSock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    bzero(&addr, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(newSock, (sockaddr *)&addr, sizeof(addr)) < 0)
        std::cout << "error\n";
    if (listen(newSock, 5) < 0)
        std::cout << "error\n";
    if (epoll_ctl_add(epfd, newSock, EPOLLIN | EPOLLOUT | EPOLLET) < 0)
        std::cout << "error\n";

	socklen_t s_len (sizeof(cli_addr));
	int cli_sock;
	struct epoll_event events[20];
	while (1)
	{
		int nb_fds = epoll_wait(epfd, events, 20, 5000);
		for (int i = 0;i < nb_fds; ++i)
		{
			if (events[i].data.fd == newSock)
			{
				cli_sock = accept(events[i].data.fd, (sockaddr *)&cli_addr, &s_len);
				epoll_ctl_add(epfd, cli_sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
			}
			if (events[i].events & EPOLLIN)
			{
				msg = recv(cli_sock, buffer, sizeof(buffer), 0);
				if (msg < 0)
				{
					ft_close(cli_sock);
					std::cerr << "Couldn't receive message." << std::endl;
					return (1);
				}
				for (unsigned int i = 0; i < 50; ++i)
					std::cout << buffer[i];
			}

		}
	
	}
	ft_close(cli_sock);
	return (0);
}*/

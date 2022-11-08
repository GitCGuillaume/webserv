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

void ft_close(int fd_socket)
{
	if (close(fd_socket) < 0)
	{
		std::cerr << "Couldn't close properly" << std::endl;
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

void upload_single_file(std::string &header, std::string link)
{
	std::ifstream file(link.c_str(), std::ifstream::in | std::ifstream::binary);
	std::istreambuf_iterator<char> it(file), ite;
	std::string buf(it, ite);

	header = "POST /website/cgi-bin/upload_file.php HTTP/1.1\r\n"
			 "Host: 127.0.0.1\r\n"
			 "Content-Type: multipart/form-data; boundary=\"0123456789012345678901234567890123456789012345678901234567890123456789\"\r\n"
			 "Content-Length: 1048841\r\n"
			 "Connection: keep-alive\r\n"
			 "\r\n"																			 // length start after \n
			 "--0123456789012345678901234567890123456789012345678901234567890123456789\r\n"; // 74
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"1mo\"\r\n");						  // 71
	header.insert(header.size(), "Content-Type: application/octet-stream\r\n\r\n");										  // 42
	header.insert(header.size(), buf);																					  // 1048576
	header.insert(header.size(), "\r\n--0123456789012345678901234567890123456789012345678901234567890123456789\r\n\r\n"); // 78
}

void upload_single_all(std::string link, int ft_socket)
{
	char buffer2[50000];
	std::string header;
	ssize_t msg = 0;

	upload_single_file(header, link);
	msg = send(ft_socket, header.c_str(), header.size(), 0);
	if (msg < 0)
		std::cerr << "Couldn't send message." << std::endl;
	msg = recv(ft_socket, buffer2, sizeof(buffer2), 0);
	if (msg < 0)
	{
		ft_close(ft_socket);
		std::cerr << "Couldn't receive message." << std::endl;
		return;
	}
	std::cout << buffer2 << std::endl;
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
	upload_single_all("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/1mo", ft_socket);
	ft_close(ft_socket);
	return (0);
}
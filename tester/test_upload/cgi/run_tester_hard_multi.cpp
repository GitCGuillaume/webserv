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

void upload_multiple_file2(std::string &header)
{
	std::ifstream file("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/1go", std::ifstream::in | std::ifstream::binary);
	std::ifstream file2("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/1go", std::ifstream::in | std::ifstream::binary);
	std::istreambuf_iterator<char> it(file), ite;
	std::istreambuf_iterator<char> it2(file2), ite2;
	std::string buf(it, ite);
	std::string buf2(it2, ite2);

	header = "POST /website/cgi-bin/upload_file.php HTTP/1.1\r\n"
			 "Host: 127.0.0.1\r\n"
			 "Content-Type: multipart/form-data; boundary=\"myboundary\"\r\n"
			 "Content-Length: 4294967831\r\n"
			 "Connection: keep-alive\r\n"
			 "\r\n"				 // length start after \n
			 "--myboundary\r\n"; // 14
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"1go\"\r\n"); // 71
	header.insert(header.size(), "Content-Type: application/octet-stream\r\n\r\n");					// 42
	header.insert(header.size(), buf);																// 1073741824
	header.insert(header.size(), "\r\n--myboundary\r\n");											// 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"1go2\"\r\n"); // 72
	header.insert(header.size(), "Content-Type: application/octet-stream\r\n\r\n");					 // 42
	header.insert(header.size(), buf2);																 // 1073741824
	header.insert(header.size(), "\r\n--myboundary\r\n");											 // 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"1go3\"\r\n"); // 72
	header.insert(header.size(), "Content-Type: application/octet-stream\r\n\r\n");					 // 42
	header.insert(header.size(), buf2);																 // 1073741824
	header.insert(header.size(), "\r\n--myboundary\r\n");											 // 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"1go4\"\r\n"); // 72
	header.insert(header.size(), "Content-Type: application/octet-stream\r\n\r\n");					 // 42
	header.insert(header.size(), buf2);																 // 1073741824
	header.insert(header.size(), "\r\n--myboundary\r\n\r\n");										 // 18
}

void upload_multiple_file(std::string &header)
{
	std::ifstream file("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/1go", std::ifstream::in | std::ifstream::binary);
	std::ifstream file2("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/1go", std::ifstream::in | std::ifstream::binary);
	std::istreambuf_iterator<char> it(file), ite;
	std::istreambuf_iterator<char> it2(file2), ite2;
	std::string buf(it, ite);
	std::string buf2(it2, ite2);

	header = "POST /website/upload/ HTTP/1.1\r\n"
			 "Host: 127.0.0.1\r\n"
			 "Content-Type: multipart/form-data; boundary=\"myboundary\"\r\n"
			 "Content-Length: 2147483923\r\n"
			 "Connection: keep-alive\r\n"
			 "\r\n"				 // length start after \n
			 "--myboundary\r\n"; // 14
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"1go\"\r\n"); // 71
	header.insert(header.size(), "Content-Type: application/octet-stream\r\n\r\n");					// 42
	header.insert(header.size(), buf);																// 1073741824
	header.insert(header.size(), "\r\n--myboundary\r\n");											// 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"1go2\"\r\n"); // 72
	header.insert(header.size(), "Content-Type: application/octet-stream\r\n\r\n");					 // 42
	header.insert(header.size(), buf2);																 // 1073741824
	header.insert(header.size(), "\r\n--myboundary\r\n\r\n");										 // 18
}

int main(int argc, char **argv)
{
	ssize_t msg = 0;
	uint16_t port = 0;
	int ft_connect = 0;
	int ft_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::string header;

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
	upload_multiple_file(header);
	msg = send(ft_socket, header.c_str(), header.size(), 0);
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
	header.clear();
	upload_multiple_file2(header);
	msg = send(ft_socket, header.c_str(), header.size(), 0);
	if (msg < 0)
		std::cerr << "Couldn't send message." << std::endl;
	msg = recv(ft_socket, buffer, sizeof(buffer), 0);
	if (msg < 0)
	{
		ft_close(ft_socket);
		std::cerr << "Couldn't receive message." << std::endl;
		return (1);
	}
	std::cout << buffer << std::endl;
	ft_close(ft_socket);
	return (0);
}
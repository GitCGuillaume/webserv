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

	// header = "POST /website/cgi-bin/upload_file.php HTTP/1.1\r\n"
	header = "POST /website/upload/ HTTP/1.1\r\n"
			 "Host: 127.0.0.1\r\n"
			 "Content-Type: multipart/form-data; boundary=\"myboundary\"\r\n"
			 "Content-Length: 203\r\n"
			 "Connection: keep-alive\r\n"
			 "\r\n"				 // length start after \n
			 "--myboundary\r\n"; // 14
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test_boundary2.txt\"\r\n"); // 86
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");											   // 28
	header.insert(header.size(), buf);																			   // 57
	header.insert(header.size(), "\r\n--myboundary\r\n\r\n");													   // 18
}

void upload_multiple_file(std::string &header)
{
	std::ifstream file("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/test_boundary.txt", std::ifstream::in | std::ifstream::binary);
	std::ifstream file2("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/test_boundary2.txt", std::ifstream::in | std::ifstream::binary);
	std::istreambuf_iterator<char> it(file), ite;
	std::istreambuf_iterator<char> it2(file2), ite2;
	std::string buf(it, ite);
	std::string buf2(it2, ite2);

	// header = "POST /website/cgi-bin/upload_file.php HTTP/1.1\r\n"
	header = "POST /website/upload/ HTTP/1.1\r\n"
			 "Host: 127.0.0.1\r\n"
			 "Content-Type: multipart/form-data; boundary=\"myboundary\"\r\n"
			 "Content-Length: 387\r\n"
			 "Connection: keep-alive\r\n"
			 "\r\n"				 // length start after \n
			 "--myboundary\r\n"; // 14
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test_boundary.txt\"\r\n"); // 85
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");											  // 28
	header.insert(header.size(), buf);																			  // 55
	header.insert(header.size(), "\r\n--myboundary\r\n");														  // 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test_boundary2.txt\"\r\n"); // 86
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");											   // 28
	header.insert(header.size(), buf2);																			   // 57
	header.insert(header.size(), "\r\n--myboundary\r\n\r\n");													   // 18
}

void upload_multiple_file_wrong(std::string &header)
{
	std::ifstream file("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/test_boundary.txt", std::ifstream::in | std::ifstream::binary);
	std::ifstream file2("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/test_boundary2.txt", std::ifstream::in | std::ifstream::binary);
	std::ifstream file3("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/test_boundary3.txt", std::ifstream::in | std::ifstream::binary);
	std::ifstream file4("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/test_boundary4.txt", std::ifstream::in | std::ifstream::binary);
	std::istreambuf_iterator<char> it(file), ite;
	std::istreambuf_iterator<char> it2(file2), ite2;
	std::istreambuf_iterator<char> it3(file3), ite3;
	std::istreambuf_iterator<char> it4(file4), ite4;
	std::string buf(it, ite);
	std::string buf2(it2, ite2);
	std::string buf3(it3, ite3);
	std::string buf4(it4, ite4);

	// header = "POST /website/upload/ HTTP/1.1\r\n"
	header = "POST /website/cgi-bin/upload_file.php HTTP/1.1\r\n"
			 "Host: 127.0.0.1\r\n"
			 "Content-Type: multipart/form-data; boundary=\"myboundary\"\r\n"
			 "Content-Length: 1040\r\n"
			 "Connection: keep-alive\r\n"
			 "\r\n"				 // length start after \n
			 "--myboundary\r\n"; // 14
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test_boundary.txt\"\r\n"); // 85
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");											  // 28
	header.insert(header.size(), buf);																			  // 55
	header.insert(header.size(), "\r\n--myboundary\r\n");														  // 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test_boundary2.txt\"\r\n"); // 86
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");											   // 28
	header.insert(header.size(), buf2);																			   // 57
	header.insert(header.size(), "\r\n--myboundary\r\n");														   // 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test_boundary3.txt\"\r\n"); // 86
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");											   // 28
	header.insert(header.size(), buf3);																			   // 195
	header.insert(header.size(), "\r\n--myboundary\r\n");														   // 16
	header.insert(header.size(),
				  "Content-Disposition: form-data; name=\"upload_files[]\"; filename=\"test_boundary4.txt\"\r\n"); // 86
	header.insert(header.size(), "Content-Type: text/plain\r\n\r\n");											   // 28
	header.insert(header.size(), buf4);																			   // 198
	header.insert(header.size(), "\r\n--myboundary\r\n\r\n");													   // 18
}

void upload_single_all(std::string link, int ft_socket)
{
	char buffer2[50000];
	std::string header;
	ssize_t msg = 0;

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
	// upload_single_all("/mnt/nfs/homes/gchopin/Documents/webserv/tester/test_upload/files/test_boundary2.txt", ft_socket);
	ft_close(ft_socket);
	return (0);
}

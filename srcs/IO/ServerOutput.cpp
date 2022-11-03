#include "ServerOutput.hpp"

ServerOutput::ServerOutput(int sock): _socket(sock)
{
}

ServerOutput::~ServerOutput()
{
}

void ServerOutput::sendResponse(const Response &response)
{
    // std::string resp
    // (
    //      "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, world!"
    //  );
	std:: cout << "------response-----------" << std::endl;

    // std::cout << response << std::endl;
    std::string resp = response.seralize();
    std::cout << resp;

    send(_socket, resp.c_str(),resp.size() + 1, 0);
}





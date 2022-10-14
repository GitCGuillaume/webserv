#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
    private:
        int _socket;
        int _port;
    public:
        Client(int _port);
        Client(const Client &src);
        ~Client();

        Client &operator=(const Client &rhs);

        int getPort(void) const;
        int getSocket(void) const;
};
#endif

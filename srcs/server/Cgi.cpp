#include <Cgi.hpp>
#include <fcntl.h>
/*
Cgi::Cgi() : _content_type(""), _gateway_interface(""), _path_info(""),
    _path_translated(""), _query_string(""), _remote_addr(""), _remote_host(""),
    _request_method(""), _script_name(""), _server_name(""), _server_protocol(""),
    _content_length(""), _server_port(""), _ft_envp(NULL){}

Cgi::Cgi(std::string const & content_type , std::string const & gateway_interface,
            std::string const & path_info, std::string const & path_translated, std::string const & query_string,
            std::string const & remote_addr, std::string const & remote_host, std::string const & request_method,
            std::string const & script_name, std::string const & server_name, std::string const & server_protocol,
            std::string const & content_length, std::string const & server_port) : _content_type(content_type), _gateway_interface(gateway_interface), _path_info(path_info),
    _path_translated(path_translated), _query_string(query_string), _remote_addr(remote_addr), _remote_host(remote_host),
    _request_method(request_method), _script_name(script_name), _server_name(server_name), _server_protocol(server_protocol),
    _content_length(content_length), _server_port(server_port)
{
    _ft_envp = new char *[14];
    //_ft_envp[0] = const_cast<char*>(_content_type.c_str());
    _ft_envp[0] = const_cast<char*>(std::string("test").c_str());
    _ft_envp[1] = NULL;
    std::cout << _ft_envp[0]<<std::endl;
}

Cgi::~Cgi()
{
    delete[] _ft_envp;
}
Cgi::Cgi(Cgi const & src) : _content_type(src._content_type), _gateway_interface(src._gateway_interface), _path_info(src._path_info),
    _path_translated(src._path_translated), _query_string(src._query_string), _remote_addr(src._remote_addr), _remote_host(src._remote_host),
    _request_method(src._request_method), _script_name(src._script_name), _server_name(src._server_name), _server_protocol(src._server_protocol),
    _content_length(src._content_length), _server_port(src._server_port){}
Cgi & Cgi::operator=(Cgi const & src)
{
    if (this != &src)
    {
        _content_type = src._content_type;
        _gateway_interface = src._gateway_interface;
        _path_info = src._path_info;
        _path_translated = src._path_translated;
        _query_string = src._query_string;
        _remote_addr = src._remote_addr;
        _remote_host = src._remote_host;
        _request_method = src._request_method;
        _script_name = src._script_name;
        _server_name = src._server_name;
        _server_protocol = src._server_protocol;
        _content_length = src._content_length;
        _server_port = src._server_port;
    }
    return (*this);
}
*/

Cgi::Cgi() {}
Cgi::Cgi(std::string const body, std::string const content_type, std::string const gateway_interface,
         std::string const path_info, std::string const path_translated, std::string const query_string,
         std::string const remote_addr, std::string const remote_host, std::string const request_method,
         std::string const script_name, std::string const server_name, std::string const server_protocol,
         std::string const content_length, std::string const server_port) : _body(body)
{
    /*_ft_envp = new char *[14];
   _ft_envp[0] = const_cast<char *>(content_type.c_str());
   _ft_envp[1] = const_cast<char *>(gateway_interface.c_str());
   _ft_envp[2] = const_cast<char *>(path_info.c_str());
   _ft_envp[3] = const_cast<char *>(path_translated.c_str());
   _ft_envp[4] = const_cast<char *>(query_string.c_str());
   _ft_envp[5] = const_cast<char *>(remote_addr.c_str());
   _ft_envp[6] = const_cast<char *>(remote_host.c_str());
   _ft_envp[7] = const_cast<char *>(request_method.c_str());
   _ft_envp[8] = const_cast<char *>(script_name.c_str());
   _ft_envp[9] = const_cast<char *>(server_name.c_str());
   _ft_envp[10] = const_cast<char *>(server_protocol.c_str());
   _ft_envp[11] = const_cast<char *>(content_length.c_str());
   _ft_envp[12] = const_cast<char *>(server_port.c_str());
   _ft_envp[13] = NULL;*/
    _vec.push_back(content_type);
    _vec.push_back(gateway_interface);
    _vec.push_back(path_info);
    _vec.push_back(path_translated);
    _vec.push_back(query_string);
    _vec.push_back(remote_addr);
    _vec.push_back(remote_host);
    _vec.push_back(request_method);
    _vec.push_back(script_name);
    _vec.push_back(server_name);
    _vec.push_back(server_protocol);
    _vec.push_back(content_length);
    _vec.push_back(server_port);
}

Cgi::~Cgi()
{
    ;
}
Cgi::Cgi(Cgi const &src)
{
    _vec = src._vec;
}

Cgi &Cgi::operator=(Cgi const &src)
{
    if (this != &src)
    {
        _vec = src._vec;
    }
    return (*this);
}

void Cgi::start()
{
    pid_t pid = 0;
    int fds_child[2];
    int fds_parent[2];
    int fds_save[2];

    fds_save[0] = dup(STDIN_FILENO);
    fds_save[1] = dup(STDOUT_FILENO);
    if (pipe(fds_child) < 0)
        throw std::range_error("Error pipe");
    if (pipe(fds_parent) < 0)
        throw std::range_error("Error pipe");
    pid = fork();
    write(fds_child[1], _body.c_str(), _body.length()); // PAS SUR QUE CA REPONDE AU SUJET
    std::string request_method(_vec[8].substr(15, _vec[8].length()));
    if (pid < 0)
    {
        close(fds_child[0]);
        close(fds_child[1]);
        close(fds_parent[0]);
        close(fds_parent[1]);
        throw std::range_error("Process creation failed");
    }
    else if (pid == 0)
    {
        char *ft_argv[3] = {const_cast<char *>("/mnt/nfs/homes/gchopin/Documents/webserv/tester/www/website/cgi-bin/php-cgi"),
                            const_cast<char *>(_vec[3].c_str()), 0};
        char *ft_envp[_vec.size() + 2];
        for (unsigned int i = 0; i < 13; ++i)
            ft_envp[i] = const_cast<char *>(_vec[i].c_str());
        ft_envp[13] = const_cast<char *>("REDIRECT_STATUS=200"); // hardcoded
        ft_envp[14] = 0;
        close(fds_parent[0]);
        close(fds_child[1]);
        dup2(fds_parent[1], STDOUT_FILENO);
        dup2(fds_child[0], STDIN_FILENO);
        if (execve(ft_argv[0], ft_argv, ft_envp) < 0)
            std::cerr << "Execve CGI failed" << std::endl;
        close(fds_parent[1]);
        close(fds_child[0]);
    }
    // fcntl?
    fcntl(fds_child[0], F_SETFL, O_NONBLOCK); // Otherwise it block server
    close(fds_parent[1]);
    close(fds_child[0]);
    int wstatus = 0;
    wait(&wstatus);
    char c = 0;
    for (int i = 0; i < 1000; ++i)
    {
        read(fds_parent[0], &c, 1);
        std::cout << c;
    }
    close(fds_parent[1]);
    close(fds_child[0]);
    dup2(fds_save[0], STDIN_FILENO);
    dup2(fds_save[1], STDOUT_FILENO);
    close(fds_save[0]);
    close(fds_save[1]);
}
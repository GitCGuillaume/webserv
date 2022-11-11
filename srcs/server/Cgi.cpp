#include <Cgi.hpp>
#include <fcntl.h>
#include <cstdio>

Cgi::Cgi() {}
Cgi::Cgi(std::string const body, std::string const content_type, std::string const gateway_interface,
         std::string const path_info, std::string const path_translated, std::string const query_string,
         std::string const remote_addr, std::string const remote_host, std::string const request_method,
         std::string const script_name, std::string const server_name, std::string const server_protocol,
         std::string const content_length, std::string const server_port) : _body(body)
{
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

Cgi::~Cgi() {}
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

/* use tmpfile, because fd alone have a limit of size
    65KB on linux ? */
int Cgi::start(std::string const &_cgi_path)
{
    pid_t pid = 0;
    int wstatus = 0;
    int fds_save[2];
    std::FILE *tmp_child_in = std::tmpfile();
    std::FILE *tmp_parent_out = std::tmpfile();
    int fd_child_in = fileno(tmp_child_in);
    int fd_parent_out = fileno(tmp_parent_out);

    fds_save[0] = dup(STDIN_FILENO);
    fds_save[1] = dup(STDOUT_FILENO);
    write(fd_parent_out, _body.c_str(), _body.length());
    std::rewind(tmp_parent_out);                         // need to read from start of stream
    pid = fork();
    std::string request_method(_vec[8].substr(15, _vec[8].length()));
    if (pid < 0)
    {
        close(fd_child_in);
        close(fd_parent_out);
        throw std::range_error("Process creation failed");
    }
    else if (pid == 0)
    {
        size_t pos = _vec[3].find("=");
        char *ft_argv[3] = {const_cast<char *>(_cgi_path.c_str()),
                            const_cast<char *>(_vec[4].c_str()), 0};
        char *ft_envp[_vec.size() + 2];
        for (unsigned int i = 0; i < 13; ++i)
            ft_envp[i] = const_cast<char *>(_vec[i].c_str());
        ft_envp[13] = const_cast<char *>("REDIRECT_STATUS=200"); // hardcoded
        ft_envp[14] = 0;
        dup2(fd_child_in, STDOUT_FILENO);
        dup2(fd_parent_out, STDIN_FILENO);
        close(fd_parent_out);
        close(fd_child_in);
        if (execve(ft_argv[0], ft_argv, ft_envp) < 0)
            std::cerr << "Execve CGI failed" << std::endl;
        exit(-1);
    }
    wait(&wstatus);
    char c = 0;
    size_t length = lseek(fd_child_in, 0, SEEK_END);
    std::rewind(tmp_child_in);
    while (0 < length)
    {
        read(fd_child_in, &c, 1);
        _iss.write(&c, 1);
        c = 0;
        --length;
    }
    close(fd_parent_out);
    close(fd_child_in);
    dup2(fds_save[0], STDIN_FILENO);
    dup2(fds_save[1], STDOUT_FILENO);
    close(fds_save[0]);
    close(fds_save[1]);
    return (wstatus >> 8);
}

const std::stringstream &Cgi::getStringStream() const
{
    return (_iss);
}
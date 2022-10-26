#include "webserv.hpp"

void    open_stream_autoindex(std::list<std::string> &lst, std::stringstream &ret_html)
{
    std::ifstream ifs("/home/gchopin/Documents/webserv/srcs/response/autoindex.html", std::ifstream::in);
    std::list<std::string>::iterator it(lst.begin());
    std::list<std::string>::iterator ite(lst.end());
    std::string str;

    if (ifs.good() == false)
        throw std::runtime_error("Couldn't open autoindex.html");
    while (std::getline(ifs, str, '\n'))
    {
        ret_html << str << std::endl;
        if (str.find("<pre>") != std::string::npos)
        {
            for (; it != ite; ++it)
            {
                ret_html << "           <a href=\"" << *it << "\">" << *it << "</a>" << std::endl;
            }
        }
    }
    ifs.close();
}

void    load_directory_autoindex(std::stringstream &ret_html)
{
    std::list<std::string> lst;
    struct dirent   *dire = NULL;
    DIR*   dir = opendir("/home/gchopin/Documents/webserv/tester/www");

    if (!dir)
        throw std::runtime_error("Couldn't open autoindex directory.");
    dire = readdir(dir);
    while (dire)
    {
        lst.push_back(dire->d_name);
        dire = readdir(dir);
    }
    if (closedir(dir) < 0)
        throw std::runtime_error("Couldn't close autoindex directory.");
    open_stream_autoindex(lst, ret_html);
}
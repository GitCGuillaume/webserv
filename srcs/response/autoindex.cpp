#include "webserv.hpp"
#include <sstream>
#include <fstream>
std::list<char *> & load_directory_autoindex()
{
    std::list<char *> lst_str;
    struct dirent   *dire = NULL;
    DIR*   dir = opendir("/mnt/nfs/homes/gchopin/Documents/webserv/tester/www");

    //if (!dir)
      //  return (0);
    dire = readdir(dir);
    while (dire)
    {
        lst_str.push_back(dire->d_name);
        dire = readdir(dir);
    }
    //for (std::list<char *>::iterator it = lst_str.begin(); it != lst_str.end(); ++it)
      //  std::cout << *it << std::endl;
    //if (closedir(dir) < 0)
      //  return (0);
    //std::list<char *> *lst = &lst_str;
    return (&lst_str);
}

void    open_stream_autoindex(std::list<char *> &lst)
{
    /*if (!lst)
    {
        std::cerr << "Error : No list for autoindex available." << std::endl;
        return ;
    }*/
    /*std::list<char *>::iterator it(lst->begin());
    std::list<char *>::iterator ite(lst->end());
    for (; it != ite; ++it)
        std::cout << *it << std::endl;
    */
    for (std::list<char *>::iterator it = lst.begin(); it != lst.end(); ++it)
        std::cout << *it << std::endl;
    /*std::fstream fs("/mnt/nfs/homes/gchopin/Documents/webserv/srcs/response/autoindex.html", std::ofstream::in);
    std::list<char *>::iterator it(lst->begin());
    std::list<char *>::iterator ite(lst->end());
    std::string str;
    //std::fstream    fs;
    //fs.open("/mnt/nfs/homes/gchopin/Documents/webserv/srcs/response/autoindex.html", std::fstream::in);
    //std::cout<<fs.rdbuf();
    while (std::getline(fs, str, '\n'))
    {
        std::cout << str << std::endl;
        if (str.find("<pre>") != std::string::npos)
        {
            for (; it != ite; ++it)
                std::cout << "<a href=\"" << *it << "\">" << *it << "</a>" << std::endl;
        }
    }*/
    //fs.close();
}
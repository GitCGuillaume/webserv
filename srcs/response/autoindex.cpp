#include "webserv.hpp"

void    open_stream_autoindex(std::list<std::string> &lst, const std::string &root,  std::string &ret_html)
{
    std::ifstream ifs("srcs/response/autoindex.html", std::ifstream::in);
    std::list<std::string>::iterator it(lst.begin());
    std::list<std::string>::iterator ite(lst.end());
    std::string str;

    if (ifs.good() == false)
        throw std::runtime_error("Couldn't open autoindex.html");
    while (std::getline(ifs, str, '\n'))
    {
        ret_html += str + "\n";
        if (str.find("<title>") != std::string::npos)
        {
            ret_html += "Index of " + root + "\n";
        }
        else if (str.find("<h1>") != std::string::npos)
        {
            ret_html += "Index of " + root + "\n";
        }
        else if (str.find("<pre>") != std::string::npos)
        {
            for (; it != ite; ++it)
            {
                ret_html += "           <a href=\"" + *it + "\">" + *it + "</a>\n";
            }
        }
    }
    ifs.close();
}

void    load_directory_autoindex(std::string &ret_html, const std::string &directory, const std::string &uri)
{
    std::list<std::string> lst;
    struct dirent   *dire = NULL;
    DIR*   dir = opendir(directory.c_str());
    std::string str;

    if (!dir)
        throw std::runtime_error("Couldn't open autoindex directory.");
    dire = readdir(dir);
    while (dire)
    {
        str = std::string(dire->d_name);
        if (dire->d_type == DT_DIR && str != ".." && str != ".")
            lst.push_back(str + "/");
        else
            lst.push_back(dire->d_name);
        dire = readdir(dir);
    }
    if (closedir(dir) < 0)
        throw std::runtime_error("Couldn't close autoindex directory.");
    open_stream_autoindex(lst, uri, ret_html);
}
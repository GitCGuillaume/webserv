#include "webserv.hpp"


void open_stream_autoindex(std::set<std::string> &lst, const std::string &directory, const std::string &root, std::string &ret_html)
{
    std::ifstream ifs("srcs/response/autoindex.html", std::ifstream::in);
    std::set<std::string>::iterator it(lst.begin());
    std::set<std::string>::iterator ite(lst.end());
    std::string str;
    struct stat fileInfo;

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
                str = *it;
                std::cout << "file " << directory + str << std::endl;
                if (stat((directory + str).c_str(), &fileInfo) != 0)
                    throw std::runtime_error(strerror(errno));
                ret_html += "           <a href=\"" + str + "\">" + str + "</a>";
                ret_html.insert(ret_html.end(), 30 - str.size(), ' ');
                ret_html += std::ctime(&fileInfo.st_mtime);
                *(ret_html.end() - 1) = ' ';
                ret_html.insert(ret_html.end(), 10, ' ');
                std::ostringstream os;
                os << fileInfo.st_size;
                ret_html += os.str() + '\n';
            }
        }
    }
    ifs.close();
}

void load_directory_autoindex(std::string &ret_html, const std::string &directory, const std::string &uri)
{
    std::set<std::string> lst;
    struct dirent *dire = NULL;
    DIR *dir = opendir(directory.c_str());
    std::string str;

    if (!dir)
        throw std::runtime_error("Couldn't open autoindex directory.");
    dire = readdir(dir);
    while (dire)
    {
        str = std::string(dire->d_name);
        if (dire->d_type == DT_DIR)
            lst.insert(str + "/");
        else
            lst.insert(str);
        dire = readdir(dir);
    }
    if (closedir(dir) < 0)
        throw std::runtime_error("Couldn't close autoindex directory.");
    open_stream_autoindex(lst, directory, uri, ret_html);
}

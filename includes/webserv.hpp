#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <list>
# include <sys/types.h>          /* See NOTES */
# include <dirent.h>
# include <fstream> /* autoindex.cpp*/
# include <sstream> /* autoindex.cpp*/

void load_directory_autoindex(std::string &ret_html);

#endif

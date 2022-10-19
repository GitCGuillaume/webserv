#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <sys/types.h>          /* See NOTES */
# include <dirent.h>
# include <sys/socket.h>
# include <list> /* autoindex.cpp*/
# include <sstream> /* autoindex.cpp*/
# include <fstream> /* autoindex.cpp*/

void    load_directory_autoindex(std::stringstream &ret_html);

#endif
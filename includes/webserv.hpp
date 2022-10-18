#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <sys/types.h>          /* See NOTES */
# include <dirent.h>
# include <sys/socket.h>
#include <list>

std::list<char *> & load_directory_autoindex();
void    open_stream_autoindex(std::list<char *> &lst);

#endif
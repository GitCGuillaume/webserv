#ifndef UPLOADTOOLS
#define UPLOADTOOLS

#include <string>
#include <iostream>
#include <fstream>

bool try_upload(std::string const content_type, std::string const body, long const content_lenght);
void get_boundary(std::string const &content_type, std::string &boundary);
void get_media_type(std::string const &content_type, std::string &type,
                    std::string &sub_type);
bool use_cgi(std::string const url, std::string const type_cgi);

#endif
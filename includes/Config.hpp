#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <stdint.h>

class Config
{
private:
	struct location
	{
		std::string name;
		std::string root;
		std::string index;
		bool autoindex;
		std::map<std::string, std::string> fastcgi_param;
		std::string fastcgi_pass;
		size_t client_max_body_size;
		location() : name("/"), root("/var/lib/webserv/html/"), autoindex(false) {}
	};

	struct server
	{
		std::string name;
		std::string host;
		std::set<uint16_t> listens;
		std::map<int, std::string> error_pages;
		std::map<std::string, location> locations;
		server() : host("127.0.0.1") {}
	};

	void parse_config();
	server parse_server(size_t &pos);

	std::string _content;
	std::vector<server> _servers;

public:
	Config(const char *conf);
	Config(const Config &src);
	~Config();
	
	const std::string &getContent() const;
	const std::vector<server> &getServers() const;
};

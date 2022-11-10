#pragma once

#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <algorithm>
#include <arpa/inet.h>
#include <limits>
#include <ctype.h>
#include "Request.hpp"

class Config
{
public:
	struct server
	{
		std::string server_name;
		std::string root;
		std::string upload_path;
		bool autoindex;
		size_t client_body_limit;
		size_t read_timeout;
		size_t send_timeout;
		std::vector<std::string> index;
		std::vector<std::string> allow_methods;
		std::vector<std::pair<std::string, uint16_t> > listens;
		std::map<std::string, std::string> rewrite;
		std::map<int, std::string> error_page;
		std::map<std::string, std::string> cgi_info;
		std::map<std::string, server> locations;
		bool is_location;
		bool str_is_num(const std::string &str);
		void set_values(std::string key, std::string value);
		static bool assign_port(const std::string &str, uint16_t &val);
		std::pair<std::string, uint16_t> handle_listen(const std::string &value);
		server() : root("/var/lib/webserv/html/"), upload_path("./tester/www/upload/"), autoindex(false), client_body_limit(0), read_timeout(1), send_timeout(1), is_location(false) {}
		const server &operator=(const server &rhs)
		{
			server_name = rhs.server_name;
			root = rhs.root;
			upload_path = rhs.upload_path;
			autoindex = rhs.autoindex;
			client_body_limit = rhs.client_body_limit;
			read_timeout = rhs.read_timeout;
			send_timeout = rhs.send_timeout;
			index = rhs.index;
			allow_methods = rhs.allow_methods;
			listens = rhs.listens;
			rewrite = rhs.rewrite;
			error_page = rhs.error_page;
			cgi_info = rhs.cgi_info;
			is_location = true;
			return (*this);
		}

		friend std::ostream &operator<<(std::ostream &os, const server &rhs)
			{
				os << "{\n";
				os << "server_name: " << rhs.server_name << std::endl;
				os << "root: " << rhs.root << std::endl;
				os << "autoindex: " << rhs.autoindex << std::endl;
				os << "client_body_limit: " << rhs.client_body_limit << std::endl;
				os << "read_timeout: " << rhs.read_timeout << std::endl;
				os << "upload_path: " << rhs.upload_path << std::endl;
				for (std::vector<std::string>::const_iterator it = rhs.index.begin(); it != rhs.index.end(); ++it)
					os << "index: " << *it << std::endl;
				for (std::vector<std::string>::const_iterator it = rhs.allow_methods.begin(); it != rhs.allow_methods.end(); ++it)
					os << "allow_methods: " << *it << std::endl;
				for (std::vector<std::pair<std::string, uint16_t> >::const_iterator it = rhs.listens.begin(); it != rhs.listens.end(); ++it)
					os << "listen: " << it->first << ":" << it->second << std::endl;
				for (std::map<std::string, std::string>::const_iterator it = rhs.cgi_info.begin(); it != rhs.cgi_info.end(); ++it)
					os << "cgi_info: " << it->first <<" " << it->second << std::endl;
				for (std::map<int, std::string>::const_iterator it = rhs.error_page.begin(); it != rhs.error_page.end(); ++it)
					os << "error_page: " << it->first <<" " << it->second << std::endl;
				for (std::map<std::string, server>::const_iterator it = rhs.locations.begin(); it != rhs.locations.end() ; ++it)
					os << it->first << " " << it->second;
				os << "}\n";
				return (os);
			}


	};

	typedef std::vector<server> vect_serv;
	typedef std::vector<std::pair<std::string, uint16_t> > vect_listens;
	typedef const server *ptr_server;

	Config(const char *conf);
	Config(const Config &src);
	~Config();

	const std::string &getContent() const;
	const std::vector<server> &getServers() const;

private:
	void drop_comments();
	void parse_config();
	server parse_server(size_t *idx, std::string type, server *parent);
	std::string get_key(size_t *idx, std::string delimiter);
	static std::vector<std::string> split(std::string input, char delimiter);

	std::string _content;
	std::vector<server> _servers;

	class ConfigException : public std::exception
	{
	private:
		std::string m_error;

	public:
		ConfigException(const std::string &error) : m_error("error: " + error) {}
		~ConfigException() throw() {}
		const char *what() const throw() { return m_error.c_str(); }
	};
};
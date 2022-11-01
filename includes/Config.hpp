#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <algorithm>

#define SUCCESS 1
#define FAILED -1
#define EMPTY -2

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
		server parse_server(size_t *pos);
		int check_line_syntax(std::string line);
		std::string get_key(size_t *idx, std::string delimiter);
		std::string get_value(size_t *idx);
		size_t get_char(size_t idx, char c);
		std::string drop_comments(std::string str);

		std::string _content;
		std::vector<server> _servers;

		class ConfigException: public std::exception
		{
		private:
			std::string m_error;

		public:
			ConfigException(const std::string &fun, const std::string &error)
				: m_error(fun + ": " + error)
			{
			}
			~ConfigException() throw(){}
			const char *what() const throw() { return (m_error.c_str());}
		};

	public:

		Config(const char *conf);
		Config(const Config &src);
		~Config();
		
		const std::string &getContent() const;
		const std::vector<server> &getServers() const;
};

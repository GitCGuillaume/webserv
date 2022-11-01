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

		struct server
		{
			std::string host;
			std::string name;
			std::string root;
			std::string index;
			bool autoindex;
			std::string fastcgi_pass;
			std::map<std::string, std::string> fastcgi_param;
			size_t client_max_body_size;
			std::set<uint16_t> listens;
			std::map<int, std::string> error_pages;
			std::map<std::string, server> locations;
			server() : host("127.0.0.1"), name("/"), root("/var/lib/webserv/html/"), autoindex(false) {}
		};

		void parse_config();
		server parse_server(size_t *pos, std::string type);
		void set_values(server *server, std::string key, std::string value);
		std::string get_key(size_t *idx, std::string delimiter);
		std::string get_value(size_t *idx);
		size_t get_char(size_t idx, char c);
		std::string drop_comments(std::string str);
		std::vector<std::string> split(std::string input, char delimiter);

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

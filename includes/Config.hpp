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

class Config
{
	private:

		struct server
		{
			std::string server_name;
			std::string root;
			std::string fastcgi_pass;
			bool autoindex;
			std::vector<std::string> index;
			std::map<std::string, std::string> fastcgi_param;
			std::set<uint16_t> listens;
			std::map<int, std::string> error_page;
			std::map<std::string, server> locations;
			void init_error_page();
			server() : root("/var/lib/webserv/html/"), autoindex(false) {init_error_page();}
		};

		void drop_comments();
		void parse_config();
		server parse_server(size_t *idx, std::string type);
		std::string get_key(size_t *idx, std::string delimiter);
		void set_values(server *server, std::string key, std::string value);
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

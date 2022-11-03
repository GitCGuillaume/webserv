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
			std::vector<std::pair<std::string, uint16_t> > listens;
			std::map<int, std::string> error_page;
			std::map<std::string, server> locations;
			bool is_location;


			void init_error_page();
			static bool assign_port(const std::string &str, uint16_t &val);
			server() : root("/var/lib/webserv/html/"), autoindex(false), is_location(false) {init_error_page();}
			const server &operator=(const server &rhs)
			{
				server_name = rhs.server_name;
				root = rhs.root;
				fastcgi_pass = rhs.fastcgi_pass;
				autoindex = rhs.autoindex;
				index = rhs.index;
				error_page = rhs.error_page;
				is_location = true;
				return (*this);

			}
			friend std::ostream &operator<<(std::ostream &os, const server &rhs)
			{
				os << "{\n";
				os << "server_name: " << rhs.server_name << std::endl;
				os << "root: " << rhs.root << std::endl;
				os << "fastcgi_pass: " << rhs.fastcgi_pass << std::endl;
				os << "autoindex: " << rhs.autoindex << std::endl;
				for (std::vector<std::pair<std::string, uint16_t> >::const_iterator it = rhs.listens.begin(); it != rhs.listens.end(); ++it)
					os << "listen: " << it->first << ":" << it->second << std::endl;
				for (std::map<std::string, server>::const_iterator it = rhs.locations.begin(); it != rhs.locations.end() ; ++it)
					os << it->first << " " << it->second;
				os << "}\n";
				return (os);
			}

		};

		void drop_comments();
		void parse_config();
		server parse_server(size_t *idx, std::string type, server *parent);
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

		typedef std::vector<server> vect_serv;
		typedef std::vector<std::pair<std::string, uint16_t> > vect_listens;
		typedef const server * ptr_server;
		
		const std::string &getContent() const;
		const std::vector<server> &getServers() const;
};

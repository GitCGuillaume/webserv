#include "Config.hpp"

Config::Config(const char *conf)
{
	std::ifstream f(conf); // taking file as inputstream
	if (f)
	{
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		_content = ss.str();
	}
	parse_conf();
	//std::cout << _content;
}

Config::Config(const Config &src) : _servers(src._servers), _content(src._content) {}

Config::~Config() {}

const std::string &Config::getContent() const
{
	return _content;
}

const std::vector<Config::server> &Config::getServers() const
{
	return _servers;
}

void Config::parse_conf()
{
	size_t pre = 0;
	size_t cur = _content.find_first_not_of(" \t\n", pre);
	if (cur == std::string::npos)
		return;
	while (cur != std::string::npos)
	{
		pre = _content.find_first_not_of(" \t\n", cur);
		cur = _content.find_first_of(" \t\n", pre);
		std::string key = _content.substr(pre, cur - pre);
		if (key[0] == '#')
			cur = _content.find("\n", cur);
        else if (key == "server")
		{
            Config::server server;
		    _servers.push_back(server);
			std::cout << cur << std::endl;
        }
        else    
            return;
	}
}
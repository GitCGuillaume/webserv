#include "Config.hpp"

Config::Config(const char *conf)
{
	std::ifstream f(conf);
	if (f)
	{
		std::ostringstream ss;
		ss << f.rdbuf(); 
		_content = ss.str();
	}
	parse_config();
	std::cout << _content;
}

Config::Config(const Config &src) : _content(src._content), _servers(src._servers) {}

Config::~Config() {}

const std::string &Config::getContent() const
{
	return _content;
}

const std::vector<Config::server> &Config::getServers() const
{
	return _servers;
}

void Config::parse_config()
{
	size_t cur = 0;
	std::string key;
	while (cur != std::string::npos)
	{
		key = get_key(&cur, " \t\n");
		std::cout << "key " << key << std::endl;
		if (key[0] == '#')
			cur = _content.find("\n", cur + 1);
		else if (key == "server")
			_servers.push_back(parse_server(&cur));
		else {
			std::cout << "error" << key << std::endl;
			throw ConfigException("incorrect keyword at the beginning of line", strerror(errno));
		}
	}
	if (_servers.empty())
		throw ConfigException("no server found", strerror(errno));
}

Config::server Config::parse_server(size_t *i)
{
	size_t 	cur;
	std::string key;
	Config::server result;

	cur = _content.find_first_not_of(" \t\n", *i);
	if (cur == std::string::npos || _content[cur] != '{')
		throw ConfigException("missing open bracket in server block", strerror(errno));
	cur++;
	cur = _content.find_first_not_of(" \t\n", cur);

	while (cur != std::string::npos)
	{
		key = get_key(&cur, " \t\n");
		std::cout << "key " << key << std::endl;

		if (key[0] == '#')
			cur = _content.find("\n", cur + 1);
		if (key == "}")
		{
			*i = _content.find_first_not_of(" \n\t", cur + 1);
			break;
		}
		if (key == "location")
		{
			std::cout << key << " " << cur << std::endl;
			cur =  _content.find_first_not_of(" \t\n", get_char(cur, '}') + 1);
			std::cout << "cur end loc " << cur << std::endl;
			// result.locations.push_back(parse_location(&cur));
		}
		else
		{
			std::string value = get_value(&cur);
			std::cout << "value " << value << std::endl;

		}
	}
	return result;
}

std::string Config::get_key(size_t *idx, std::string delimiter)
{
	size_t pre = _content.find_first_not_of(" \t\n", *idx);
	if (pre == std::string::npos)
		throw ConfigException("parsing error", strerror(errno));
	size_t cur = _content.find_first_of(delimiter, pre + 1);
	if (cur == std::string::npos)
		throw ConfigException("parsing error", strerror(errno));
	std::string key = _content.substr(pre, cur - pre);
	*idx = cur;
	std::cout << "idx " << cur << std::endl;
	return key;
}

std::string Config::get_value(size_t *idx)
{
	size_t eol = get_char(*idx + 1, ';');
	std::string value = _content.substr(*idx, eol - *idx);
	*idx = eol + 1;
	return drop_comments(value);
}

std::string Config::drop_comments(std::string str) 
{
	size_t i = 0;
	while (i < str.length())
	{
		if (str[i] == '#')
			str.erase(i, str.find_first_of("\n", i + 1) - i);
		else
			i++;
	}
	return str;
}

size_t Config::get_char(size_t idx, char c)
{
	while(_content[idx] && _content[idx] != c)
	{
		if (_content[idx] == '#')
			idx = _content.find_first_of("\n", idx + 1);
		idx++;
	}
	if (_content[idx] == c)
		return idx;
	throw ConfigException("no terminating line", strerror(errno));
}
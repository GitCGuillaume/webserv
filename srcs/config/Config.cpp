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
	drop_comments();
	std::cout << _content;
	parse_config();
	for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
		std::cout << *it;
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

void Config::drop_comments() 
{
	size_t i = 0;
	size_t line = 0;
	while (i < _content.length())
	{
		if (_content[i] == '#')
		{
			line = _content.find_first_of("\n",  i + 1);
			if (line != std::string::npos)
				_content.erase(i, line - i + 1);
		}
		else
			i++;
	}
}

void Config::parse_config()
{
	size_t pos = 0;
	std::string key;
	while (pos != std::string::npos)
	{
		key = get_key(&pos, " \t\n");
		std::cout << "key " << key << std::endl;
		if (key == "server")
			_servers.push_back(parse_server(&pos, "server", NULL));
		else {
			std::cout << "error " << key << std::endl;
			throw ConfigException("incorrect keyword at start of line", strerror(errno));
		}
	}
	if (_servers.empty())
		throw ConfigException("no server found", strerror(errno));
}

Config::server Config::parse_server(size_t *idx, std::string type, server *parent)
{
	std::string key;
	size_t pos = *idx;
	Config::server res;

	if (get_key(&pos, " \t\n") != "{")
		throw ConfigException("open bracket missing in server block", strerror(errno));

	if (parent)
		res = *parent;
		
	while (pos != std::string::npos)
	{
		key = get_key(&pos, " \t\n");
		std::cout << "key: " << key << std::endl;
		if (key == "}" && (*idx = _content.find_first_not_of(" \n\t", pos + 1)))
			break;
		if (type == "server" && key == "location") {
			std::string name = get_key(&pos, " \t\n");
			std::cout << "test" << name<< std::endl;
			res.locations[name] = parse_server(&pos, "location", &res);
		}
		else
		{
			std::string value = get_key(&pos, ";");
			std::cout << "value: " << value << std::endl;
			res.set_values(key, value);  
			pos++;
		}
	}
	return res;
}

std::string Config::get_key(size_t *idx, std::string delimiter)
{
	size_t pre = _content.find_first_not_of(" \t\n", *idx);
	if (pre == std::string::npos)
		throw ConfigException("parsing error", strerror(errno));
	size_t pos = _content.find_first_of(delimiter, pre);
	if (pos == std::string::npos)
		throw ConfigException("no delimiter found", strerror(errno));
	std::string key = _content.substr(pre, pos - pre);
	*idx = pos;
	return key;
}

bool str_is_num(const std::string &str)
{
	if (str.empty())
		return (false);
	for (int i = 0; i < str.length(); ++i)
		if (!std::isdigit(str[i]))
			return false;
	return true;
}
bool Config::server::assign_port(const std::string &str, uint16_t &val)
{
	long l = std::strtol(str.c_str(), NULL, 10);
	if (l < std::numeric_limits<uint16_t>::min() || l > std::numeric_limits<uint16_t>::max() || errno == ERANGE)
		return false;
	val = static_cast<uint16_t>(l);
	return (true);
}

std::pair<std::string, uint16_t> Config::server::handle_listen(const std::string value) {
		uint16_t port = 80;
		std::string address = "0.0.0.0";
		std::vector<std::string> tmp = Config::split(value, ':');
		if (tmp.size() == 1)
		{
			if (str_is_num(tmp[0])) {
				if (!server::assign_port(value, port))			
					throw ConfigException("incorrect PORT value", "");
			}
			else if (inet_addr(tmp[0].c_str()) != (in_addr_t) -1 ) 
				address = tmp[0];
			else
				throw ConfigException("invalid IP address", "");
		}
		else if (tmp.size() == 2 && ( tmp[0].empty() || inet_addr(tmp[0].c_str()) != (in_addr_t) -1) && str_is_num(tmp[1]) && server::assign_port(tmp[1], port))
		{
			if (!tmp[0].empty())
				address = tmp[0];
		}
		else
			throw ConfigException("listen error", "");
		return 	std::make_pair(address, port); 
}

void Config::server::set_values(const std::string key, const std::string value)
{
	
	if (key == "server_name" && !this->is_location)
		this->server_name = value;
	else if (key == "root")
		this->root = value;
	else if (key == "fastcgi_pass")
		this->fastcgi_pass = value;
	else if (key == "autoindex")
		this->autoindex = value == "on" ? true : false;
	else if (key == "listen" && !this->is_location) 
		this->listens.push_back(handle_listen(value));
	else if (key == "fastcgi_param") {}
	else if (key == "index") {}
	else if (key == "client_max_body_size") {}
	else if (key == "error_page") {}
	else {
		std::cout << "key error: " << key << std::endl;
		//throw ConfigException("key error" + key, strerror(errno));
	}
}

void Config::server::init_error_page() {
	error_page[400]= "response/error_pages/400.html";

}


std::vector<std::string> Config::split(std::string input, char delimiter)
{
    std::vector<std::string> res;
    std::stringstream ss(input);
    std::string str;
    while (getline(ss, str, delimiter))
        res.push_back(str);
    return res;
}

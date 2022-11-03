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
	while (i < _content.length())
	{
		if (_content[i] == '#')
			_content.erase(i, _content.find_first_of("\n", i + 1) - i + 1);
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
			_servers.push_back(parse_server(&pos, "server"));
		else 
			throw ConfigException("incorrect keyword at start of line: " + key, strerror(errno));
	}
	if (_servers.empty())
		throw ConfigException("no server found", strerror(errno));
}

Config::server Config::parse_server(size_t *idx, std::string type)
{
	std::string key;
	size_t pos = *idx;
	Config::server res;

	if (get_key(&pos, " \t\n") != "{")
		throw ConfigException("open bracket missing in server block", strerror(errno));
		
	while (pos != std::string::npos)
	{
		key = get_key(&pos, " \t\n");
		std::cout << "key: " << key << std::endl;
		if (key == "}" && (*idx = _content.find_first_not_of(" \n\t", pos + 1)))
			break;
		if (type == "server" && key == "location") {
			std::string name = get_key(&pos, " \t\n");
			std::cout << "test" << name<< std::endl;
			res.locations[name] = parse_server(&pos, "location");
		}
		else {
			std::string value = get_key(&pos, ";");
			std::cout << "value: " << value << std::endl;
			set_values(&res, key, value);  
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

void Config::set_values(server *server, const std::string key, const std::string value)
{
	
	if (key == "server_name")
		server->server_name = value;
	else if (key == "root")
		server->root = value;
	else if (key == "fastcgi_pass")
		server->fastcgi_pass = value;
	else if (key == "autoindex")
		server->autoindex = value == "on" ? true : false;
	else if (key == "fastcgi_param") {
		std::vector<std::string> tmp = split(value, ' ');
		std::cout << "param: " << tmp[0] << ", value: " << tmp[1] << std::endl;
		server->fastcgi_param[tmp[0]] = tmp[1];
	}
	else if (key == "listen") {
		// std::vector<std::string> tmp = split(value, ':');
		// std::cout << "port: " << atoi(tmp[1].c_str()) << std::endl;
		// server->listens.insert(atoi(tmp[1].c_str()));
	}
	else if (key == "error_page") {
		// server->error_page[]
	}
	else {
		std::cout << "key error: " << key << std::endl;
		// throw ConfigException("key error" + key, strerror(errno));
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
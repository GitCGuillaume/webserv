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
	else
		throw ConfigException("failed to read file");
	drop_comments();
	parse_config();
	for (std::vector<server>::iterator it_s = _servers.begin(); it_s != _servers.end(); ++it_s)
	{
		std::cout << "server_name: " << it_s->server_name << std::endl;
		for (std::vector<std::pair<std::string, uint16_t>>::const_iterator it = it_s->listens.begin(); it != it_s->listens.end(); ++it)
			std::cout << "listen: " << it->first << ":" << it->second << std::endl;
	}
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
			line = _content.find_first_of("\n", i + 1);
			if (line != std::string::npos)
				_content.erase(i, line - i);
			else
				_content.erase(i, _content.length() - i);
		}
		if (_content[i] == '\t' || _content[i] == '\n')
			_content[i] = ' ';
		i++;
	}
}

void Config::parse_config()
{
	size_t pos = 0;
	std::string key;
	while (pos != std::string::npos)
	{
		key = get_key(&pos, " {");
		if (key == "server")
			_servers.push_back(parse_server(&pos, "server", NULL));
		else
			throw ConfigException("incorrect keyword at start of line" + key);
	}
	if (_servers.empty())
		throw ConfigException("no server found");
}

Config::server Config::parse_server(size_t *idx, std::string type, server *parent)
{
	std::string key;
	size_t pos = *idx;
	Config::server res;

	if ((pos = _content.find_first_not_of(" ", pos)) && _content[pos++] != '{')
		throw ConfigException("open bracket missing in server block");

	if (parent)
		res = *parent;

	while (pos != std::string::npos)
	{
		key = get_key(&pos, " ");
		if (key[0] == '}' && (*idx = _content.find_first_not_of(" ", pos + 1 - key.length())))
			break;
		if (type == "server" && key == "location")
		{
			std::string name = get_key(&pos, " ");
			if (name[name.size() - 1] != '/')
				name += "/";
			res.locations[name] = parse_server(&pos, "location", &res);
		}
		else
		{
			std::string value = get_key(&pos, ";");
			res.set_values(key, value);
			pos++;
		}
	}
	return res;
}

std::string Config::get_key(size_t *idx, std::string delimiter)
{
	size_t pre = _content.find_first_not_of(" ", *idx);
	if (pre == std::string::npos)
		throw ConfigException("parsing error");
	size_t pos = _content.find_first_of(delimiter, pre);
	if (pos == std::string::npos)
		throw ConfigException("no delimiter found");
	std::string key = _content.substr(pre, pos - pre);
	*idx = pos;
	return key;
}

bool Config::server::str_is_num(const std::string &str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it))
		++it;
	return !str.empty() && it == str.end();
	// return str.find_first_not_of("0123456789") == std::string::npos;
}

bool Config::server::assign_port(const std::string &str, uint16_t &val)
{
	long l = std::strtol(str.c_str(), NULL, 10);
	if (l < std::numeric_limits<uint16_t>::min() || l > std::numeric_limits<uint16_t>::max() || errno == ERANGE)
		return false;
	val = static_cast<uint16_t>(l);
	return (true);
}

std::pair<std::string, uint16_t> Config::server::handle_listen(const std::string &value)
{
	uint16_t port = 80;
	std::string address = "0.0.0.0";
	std::vector<std::string> tmp = Config::split(value, ':');
	if (tmp.size() == 1)
	{
		if (str_is_num(tmp[0]))
		{
			if (!server::assign_port(value, port))
				throw ConfigException("incorrect PORT value");
		}
		else if (inet_addr(tmp[0].c_str()) != (in_addr_t)-1)
			address = tmp[0];
		else
			throw ConfigException("invalid IP address");
	}
	else if (tmp.size() == 2 && (tmp[0].empty() || inet_addr(tmp[0].c_str()) != (in_addr_t)-1) && str_is_num(tmp[1]) && server::assign_port(tmp[1], port))
	{
		if (!tmp[0].empty())
			address = tmp[0];
	}
	else
		throw ConfigException("invalid listen: " + value);
	return std::make_pair(address, port);
}

void Config::server::set_values(const std::string key, const std::string value)
{
	char *p;
	std::vector<std::string> values = split(value, ' ');
	if (key == "server_name" && !this->is_location)
		this->server_name = values[0];
	else if (key == "root")
		this->root = values[0];
	else if (key == "upload_path")
		this->upload_path = values[0];
	else if (key == "autoindex")
		this->autoindex = values[0] == "on" ? true : false;
	else if (key == "listen" && !this->is_location)
		this->listens.push_back(handle_listen(value));
	else if (key == "client_max_body_size")
	{
		if (!str_is_num(value))
			throw ConfigException("invalid client_max_body_size: " + value);
		this->client_body_limit = strtoul(value.c_str(), NULL, 10);
		if (errno == ERANGE)
			throw ConfigException("invalid client_max_body_size: " + value);
	}
	else if (key == "read_timeout")
	{
		this->read_timeout = strtoul(value.c_str(), &p, 10);
		if (*p || errno == ERANGE)
			throw ConfigException("invalid read_timeout: " + value);
	}
	else if (key == "send_timeout")
	{
		this->send_timeout = strtoul(value.c_str(), &p, 10);
		if (*p || errno == ERANGE)
			throw ConfigException("invalid send_timeout: " + value);
	}
	else if (key == "index")
		this->index.insert(this->index.end(), values.begin(), values.end());
	else if (key == "allow_methods")
	{
		for (size_t i = 0; i != values.size(); i++)
		{
			if (!Request::is_method(values[i]))
				throw ConfigException("allow_methods: " + value);
			this->allow_methods.push_back(values[i]);
		}
	}
	else if (key == "error_page")
	{
		int status_code;
		if (values.size() < 2)
			throw ConfigException("invalid error_page: " + value);
		for (size_t i = 0; i != values.size() - 1; i++)
		{
			if (!str_is_num(values[i]) || !(status_code = atoi(values[i].c_str())) || this->error_page.find(status_code) == this->error_page.end())
				throw ConfigException("invalid error_page: " + values[i]);
			this->error_page[status_code] = *(values.end() - 1);
		}
	}
	else if (key == "cgi_info")
	{
		if (values.size() != 2)
			throw ConfigException("invalid cgi_info: " + value);
		this->cgi_info[values[0]] = values[1];
	}
	else
		throw ConfigException("invalid key: " + key);
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
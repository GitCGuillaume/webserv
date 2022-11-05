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

	if (get_key(&pos, " \t\n") != "{")
		throw ConfigException("open bracket missing in server block");

	if (parent)
		res = *parent;
		
	while (pos != std::string::npos)
	{
		key = get_key(&pos, " \t\n");
		if (key == "}" && (*idx = _content.find_first_not_of(" \n\t", pos + 1)))
			break;
		if (type == "server" && key == "location") {
			std::string name = get_key(&pos, " \t\n");
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
	size_t pre = _content.find_first_not_of(" \t\n", *idx);
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
	if (str.empty())
		return (false);
	for (size_t i = 0; i < str.length(); ++i)
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

std::pair<std::string, uint16_t> Config::server::handle_listen(const std::string &value) {
		uint16_t port = 80;
		std::string address = "0.0.0.0";
		std::vector<std::string> tmp = Config::split(value, ':');
		if (tmp.size() == 1)
		{
			if (str_is_num(tmp[0])) {
				if (!server::assign_port(value, port))			
					throw ConfigException("incorrect PORT value");
			}
			else if (inet_addr(tmp[0].c_str()) != (in_addr_t) -1 ) 
				address = tmp[0];
			else
				throw ConfigException("invalid IP address");
		}
		else if (tmp.size() == 2 && ( tmp[0].empty() || inet_addr(tmp[0].c_str()) != (in_addr_t) -1) && str_is_num(tmp[1]) && server::assign_port(tmp[1], port))
		{
			if (!tmp[0].empty())
				address = tmp[0];
		}
		else
			throw ConfigException("invalid listen: " + value);
		return 	std::make_pair(address, port); 
}


void Config::server::set_values(const std::string key, const std::string value)
{
	
	if (key == "server_name" && !this->is_location)
		this->server_name = value;
	else if (key == "root")
		this->root = value;
	else if (key == "upload_path")
		this->upload_path = value;
	else if (key == "autoindex")
		this->autoindex = value == "on" ? true : false;
	else if (key == "listen" && !this->is_location) 
		this->listens.push_back(handle_listen(value));
	else if (key == "client_max_body_size") {
		if (!str_is_num(value) || (client_body_limit = strtoul(value.c_str(), NULL, 10) && false) || errno == ERANGE)
			throw ConfigException("invalid client_max_body_size: " + value);
	}
	else if (key == "index") {
		std::vector<std::string> tmp = split(value, ' ');
		this->index.insert(this->index.end(), tmp.begin(), tmp.end());
	}
	else if (key == "allow_methods") {
		std::vector<std::string> tmp = split(value, ' ');
		for (size_t i = 0; i != tmp.size(); i++) {
			if  (!Request::is_method(tmp[i]))
				throw ConfigException("allow_methods: " + value);
			this->allow_methods.push_back(tmp[i]);
		}
	}
	else if (key == "error_page") {
		int status_code;
		std::vector<std::string> tmp = split(value, ' ');
		if (tmp.size() < 2)
			throw ConfigException("invalid error_page: " + value);
		for (size_t i = 0; i != tmp.size() - 1; i++) {
			if (!str_is_num(tmp[i]) || !(status_code = atoi(tmp[i].c_str())) || this->error_page.find(status_code) == this->error_page.end() )
				throw ConfigException("invalid error_page: " + tmp[i]);
			this->error_page[status_code] = *(tmp.end() - 1);
		}
	}
	else if (key == "cgi_info") {
		std::vector<std::string> tmp = split(value, ' ');
		if (tmp.size() != 2)
			throw ConfigException("invalid cgi_info: " + value);
		this->cgi_info[tmp[0]] =  tmp[1];
	}
	else {
		std::cout << "key error: " << key << std::endl;
		//throw ConfigException("invalid key: " + key);
	}
}

void Config::server::init_error_page() {
	error_page[400]= "response/error_pages/400.html";
	error_page[401]= "response/error_pages/401.html";
	error_page[402]= "response/error_pages/402.html";
	error_page[403]= "response/error_pages/403.html";
	error_page[404]= "response/error_pages/404.html";
	error_page[405]= "response/error_pages/405.html";
	error_page[406]= "response/error_pages/406.html";
	error_page[407]= "response/error_pages/407.html";
	error_page[408]= "response/error_pages/408.html";
	error_page[409]= "response/error_pages/409.html";
	error_page[410]= "response/error_pages/410.html";
	error_page[411]= "response/error_pages/411.html";
	error_page[412]= "response/error_pages/412.html";
	error_page[413]= "response/error_pages/413.html";
	error_page[414]= "response/error_pages/414.html";
	error_page[415]= "response/error_pages/415.html";
	error_page[416]= "response/error_pages/416.html";
	error_page[417]= "response/error_pages/417.html";
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
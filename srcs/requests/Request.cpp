#include "Request.hpp"

Request::Request() : _is_ready(false), _code(0)
{
	init_map_headers();
}

Request::Request(const std::string &req) : _req(req), _is_ready(false), _code(0)
{
	init_map_headers();
}

Request::Request(const Request &src) : _req(src._req), _map_headers(src._map_headers), _header(src._header)
{
}

Request &Request::operator=(const Request &rhs)
{
	if (&rhs != this)
	{
		_req = rhs._req;
	}
	return (*this);
}

Request &Request::operator=(const std::string &rhs)
{
	_req = rhs;
	return (*this);
}

Request &Request::operator+=(const char *rhs)
{
	return (*this);
}

std::ostream &operator<<(std::ostream &os, const Request &rhs)
{
	os << rhs._req;
	os << "-------\n";
	os << "method: " << rhs._method << std::endl;
	os << "url: " << rhs._url << std::endl;
	os << "version: " << rhs._version << std::endl;
	os << "content-type: " << rhs._header.content_type << std::endl;
	os << "content-length: " << rhs._header.content_length << std::endl;
	os << "date: " << rhs._header.date << std::endl;
	os << "host: " << rhs._header.host << std::endl;
	os << "transfer-encoding: " << rhs._header.transfer_encoding << std::endl;
	os << "-------\n";

	return (os);
}

Request::~Request()
{
}

bool Request::is_method(const std::string &str)
{
	static std::set<std::string> methods;

	if (methods.empty())
	{
		methods.insert("GET");
		methods.insert("POST");
		methods.insert("DELETE");
	}
	return (methods.find(str) != methods.end());
}

size_t Request::parse(void)
{
	_req = ss.str();
	size_t start = 0;
	size_t prec = 0;
	if ((start = parse_request_line()) == std::string::npos)
	{
		_code = 400;
		return 0;
	}
	while (start != std::string::npos)
	{
		prec = start;
		start = parse_header(start);
	}

	if (_header.content_length < 0)
	{
		_is_ready = true;
		std::cout << "READY" << std::endl;
	}
	return prec + 2;
}

size_t Request::parse_request_line(void)
{
	size_t pos = _req.find(" ");
	if (pos == std::string::npos)
		return (pos);
	_method = _req.substr(0, pos);
	if (!is_method(_method))
	{
		_code = 400;
		return (pos);
	}
	size_t pos2 = _req.find(" ", pos + 1);
	if (pos2 == std::string::npos)
		return (pos2);
	_url = _req.substr(pos + 1, pos2 - pos);
	pos = _req.find("\r\n", pos2 + 1);
	if (pos == std::string::npos)
		return (pos);
	_version = _req.substr(pos2 + 1, pos - pos2);
	return (pos + 2);
}

size_t Request::parse_header(size_t start)
{
	size_t pos = _req.find(": ", start);
	if (pos == std::string::npos)
		return (pos);
	// std::cout<<"pos:"<<pos<<std::endl;
	std::string field_name = _req.substr(start, pos - start);
	// std::cout << "here " << field_name << std::endl;
	transform(field_name.begin(), field_name.end(), field_name.begin(), ::tolower);
	start = pos + 2;
	pos = _req.find("\r\n", start);
	if (pos == std::string::npos)
		return (pos);
	std::string field_value = _req.substr(start, pos - start);
	if (_map_headers.find(field_name) != _map_headers.end()) // else bad request
	{
		if (field_name == "content-length")
			*static_cast<size_t *>(_map_headers[field_name]) = std::strtoul(field_value.c_str(), NULL, 10);
		else
			*static_cast<std::string *>(_map_headers[field_name]) = field_value;
	}
	return (pos + 2);
}

size_t Request::parse_body(size_t start)
{
	std::cout << _req << std::endl;
	ss.seekg(start);
	std::string body = ss.str();
	std::cout << body;
	_is_ready = true;
	return (0);
}

void Request::reset(void)
{
	new (this) Request;
}

void Request::append_data(const char *data, size_t n)
{
	ss.write(data, n);
}

void Request::init_map_headers(void)
{
	_map_headers["content-type"] = &_header.content_type;
	_map_headers["content-length"] = &_header.content_length;
	_map_headers["date"] = &_header.date;
	_map_headers["host"] = &_header.host;
	_map_headers["transfer_encoding"] = &_header.transfer_encoding;
}

const std::string &Request::getReq(void)
{
	_req = ss.str();
	return (_req);
}

bool Request::is_ready() const
{
	return (_is_ready);
}

const Request::t_header &Request::getHeader() const
{
	return (_header);
}

size_t Request::size()
{
	size_t size = 0;
	if (ss)
	{
		// get length of file:
		size_t old = ss.tellg();
		ss.seekg(0, ss.end);
		size = ss.tellg();
		ss.seekg(old);
	}
	return (size);
}

#include "Request.hpp"

Request::Request() : _is_ready(false), _code(0)
{
}

Request::Request(const std::string &req) : _req(req), _is_ready(false), _code(0)
{
}

Request::Request(const Request &src) : _req(src._req), _ge_header(src._ge_header), _re_header(src._re_header), _en_header(src._en_header)
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
	os << "----req---\n";
	if (!rhs._method.empty())
		os << "method: " << rhs._method << std::endl;
	if (!rhs._url.empty())
		os << "url: " << rhs._url << std::endl;
	if (!rhs._version.empty())
		os << "version: " << rhs._version << std::endl;
	os << rhs._ge_header.toString();
	os << rhs._re_header.toString();
	os << rhs._en_header.toString();
	os << rhs._body;
	os << "---req----\n";
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

	if (_en_header.content_length.empty())
	{
		_is_ready = true;
		// std::cout << "READY " << std::endl;
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
	_url = _req.substr(pos + 1, pos2 - pos - 1);
	pos = _req.find("\r\n", pos2 + 1);
	if (pos == std::string::npos)
		return (pos);
	_version = _req.substr(pos2 + 1, pos - pos2 - 1);
	return (pos + 2);
}

size_t Request::parse_header(size_t start)
{
	size_t pos = _req.find(": ", start);
	if (pos == std::string::npos)
		return (pos);
	// std::cout<<"pos:"<<pos<<std::endl;
	std::string field_name = _req.substr(start, pos - start);
	transform(field_name.begin(), field_name.end(), field_name.begin(), ::tolower);
	start = pos + 2;
	pos = _req.find("\r\n", start);
	if (pos == std::string::npos)
		return (pos);
	std::string field_value = _req.substr(start, pos - start);
	// std::cout << "here " << field_name << std::endl;
	fillHeader(field_name, field_value); // else bad request

	return (pos + 2);
}

size_t Request::parse_body(size_t start)
{
	// std::cout << _req << std::endl;
	//ss.seekg(start);
	_body = ss.str().substr(start);
	//std::cout << body;
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

bool Request::fillHeader(const std::string &field_name, const std::string &field_value)
{
	if (s_general_header::__map_ge_headers_ptr.find(field_name) != s_general_header::__map_ge_headers_ptr.end())
	{
		_ge_header.*s_general_header::__map_ge_headers_ptr[field_name] = field_value;
		return (true);
	}
	else if (s_request_header::_map_re_headers_ptr.find(field_name) != s_request_header::_map_re_headers_ptr.end())
	{
		_re_header.*s_request_header::_map_re_headers_ptr[field_name] = field_value;
		return (true);
	}
	else if (s_entity_header::__map_en_headers_ptr.find(field_name) != s_entity_header::__map_en_headers_ptr.end())
	{
		_en_header.*s_entity_header::__map_en_headers_ptr[field_name] = field_value;
		return (true);
	}
	return (false);
}

const std::string &Request::getReq(void)
{
	_req = ss.str();
	return (_req);
}

const std::string &Request::getMethod(void) const
{
	return (_method);
}

const std::string &Request::getUrl(void) const
{
	return (_url);
}

const std::string &Request::getVersion(void) const
{
	return (_version);
}

bool Request::is_ready() const
{
	return (_is_ready);
}

// const Request::t_header &Request::getHeader() const
// {
// 	return (_header);
// }

const s_entity_header &Request::getEntityHeader() const
{
	return (_en_header);
}

size_t Request::getContentLength() const
{
	return (std::strtoul(_en_header.content_length.c_str(), NULL, 10));
}

const std::string &Request::getBody() const
{
	return (_body);
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

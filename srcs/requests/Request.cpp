#include "Request.hpp"
#include "Client.hpp"

Request::Request(const Client &client) : _is_ready(false), _client(client), _is_timeout(false), _size(0)
{
	set_time();
}

Request::Request(const Request &src) : _req(src._req), _method(src._method), _url(src._url), _version(src._version), _ge_header(src._ge_header), _re_header(src._re_header), _en_header(src._en_header), _body(src._body), _is_ready(src._is_ready), _client(src._client), _time(src._time), _is_timeout(src._is_timeout), _size(src._size)
{
	set_time();
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
	std::string field_name = _req.substr(start, pos - start);
	transform(field_name.begin(), field_name.end(), field_name.begin(), ::tolower);
	start = pos + 2;
	pos = _req.find("\r\n", start);
	if (pos == std::string::npos)
		return (pos);
	std::string field_value = _req.substr(start, pos - start);
	fillHeader(field_name, field_value); // else bad request

	return (pos + 2);
}

size_t Request::parse_body(size_t start)
{
	_body = ss.str().substr(start);
	_is_ready = true;
	return (0);
}

void Request::reset(void)
{
	new (this) Request(_client);
}

void Request::append_data(const char *data, size_t n)
{
	ss.write(data, n);
	_size += n;
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

size_t Request::size() const
{
	return (_size);
}

const Client &Request::getClient() const
{
	return (_client);
}

const std::pair<std::string, uint16_t> &Request::getIp() const
{
	return (_client.getIp());
}

timeval Request::get_time() const
{
	return _time;
}

void Request::set_time()
{
	gettimeofday(&_time, NULL);
}

bool Request::is_timeout() const
{
	return _is_timeout;
}

void Request::set_timeout()
{
	timeval tv;
	gettimeofday(&tv, NULL);

	//std::cout << "time " << (static_cast<double>(tv.tv_sec + (tv.tv_usec / 1000000.0) - (_time.tv_sec + (_time.tv_usec / 1000000.0)))) << "\n";
	if (static_cast<double>(tv.tv_sec + (tv.tv_usec / 1000000.0) - (_time.tv_sec + (_time.tv_usec / 1000000.0))) > _client.get_conf()->read_timeout)
	{
		_is_timeout = true;
	}
}

void Request::setReady()
{
	_is_ready = true;
}
#include "Header.hpp"

std::map<std::string, std::string s_general_header::*> s_general_header::_map_ge_headers_ptr;
void s_general_header::init_map_ge_headers()
{
	s_general_header::_map_ge_headers_ptr["date"] = &s_general_header::date;
	s_general_header::_map_ge_headers_ptr["transfer-encoding"] = &s_general_header::transfer_encoding;
}

std::string s_general_header::toString(void) const
{
    std::string s;
    if (!date.empty())
        s += "Date: " + date + "\r\n";
    if (!transfer_encoding.empty())
        s += "Transfer-Encoding: " + transfer_encoding + "\r\n";
    return (s);
}

std::map<std::string, std::string s_request_header::*> s_request_header::_map_re_headers_ptr;
void s_request_header::init_map_re_headers()
{
	s_request_header::_map_re_headers_ptr["host"] = &s_request_header::host;
}

std::string s_request_header::toString(void) const
{
    std::string s;
    if (!host.empty())
        s += "Host: " + host + "\r\n";
    return (s);
}

std::string s_response_header::toString(void) const
{
    std::string s;
    if (!etag.empty())
        s += "ETag: " + etag + "\r\n";
    if (!server.empty())
        s += "Server: " + server + "\r\n";
    return (s);
}

std::map<std::string, std::string s_entity_header::*> s_entity_header::_map_en_headers_ptr;
void s_entity_header::init_map_en_headers()
{
	s_entity_header::_map_en_headers_ptr["content-type"] = &s_entity_header::content_type;
	s_entity_header::_map_en_headers_ptr["content-length"] = &s_entity_header::content_length;
}

std::string s_entity_header::toString(void) const
{
    std::string s;
    if (!content_type.empty())
        s += "Content-Type: " + content_type + "\r\n";
    if (!content_length.empty())
        s += "Content-Length: " + content_length + "\r\n";
    return (s);
}
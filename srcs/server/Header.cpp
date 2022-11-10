#include "Header.hpp"

std::map<std::string, std::string s_general_header::*> s_general_header::__map_ge_headers_ptr;
void s_general_header::init_map_ge_headers()
{
    s_general_header::__map_ge_headers_ptr["date"] = &s_general_header::date;
    s_general_header::__map_ge_headers_ptr["transfer-encoding"] = &s_general_header::transfer_encoding;
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
    if (!location.empty())
        s += "Location: " + location + "\r\n";
    return (s);
}

std::map<std::string, std::string s_entity_header::*> s_entity_header::__map_en_headers_ptr;
void s_entity_header::init_map_en_headers()
{
    s_entity_header::__map_en_headers_ptr["content-type"] = &s_entity_header::content_type;
    s_entity_header::__map_en_headers_ptr["content-length"] = &s_entity_header::content_length;
}

std::map<std::string, std::string> s_entity_header::__map_ext_mime;
std::map<std::string, std::string> s_entity_header::__map_mime_ext;
void s_entity_header::init_map_mime(void)
{
    __map_ext_mime[".aac"] = "audio/aac";
    __map_ext_mime[".abw"] = "application/x-abiword";
    __map_ext_mime[".arc"] = "application/x-freearc";
    __map_ext_mime[".avif"] = "image/avif";
    __map_ext_mime[".avi"] = "video/x-msvideo";
    __map_ext_mime[".azw"] = "application/vnd.amazon.ebook";
    __map_ext_mime[".bin"] = "application/octet-stream";
    __map_ext_mime[".bmp"] = "image/bmp";
    __map_ext_mime[".bz"] = "application/x-bzip";
    __map_ext_mime[".bz2"] = "application/x-bzip2";
    __map_ext_mime[".cda"] = "application/x-cdf";
    __map_ext_mime[".csh"] = "application/x-csh";
    __map_ext_mime[".css"] = "text/css";
    __map_ext_mime[".csv"] = "text/csv";
    __map_ext_mime[".doc"] = "application/msword";
    __map_ext_mime[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    __map_ext_mime[".eot"] = "application/vnd.ms-fontobject";
    __map_ext_mime[".epub"] = "application/epub+zip";
    __map_ext_mime[".gz"] = "application/gzip";
    __map_ext_mime[".gif"] = "image/gif";
    __map_ext_mime[".htm"] = "text/html";
    __map_ext_mime[".html"] = "text/html";
    __map_ext_mime[".ico"] = "image/vnd.microsoft.icon";
    __map_ext_mime[".ics"] = "text/calendar";
    __map_ext_mime[".jar"] = "application/java-archive";
    __map_ext_mime[".jpg"] = "image/jpeg";
    __map_ext_mime[".jpeg"] = "image/jpeg";
    __map_ext_mime[".js"] = "text/javascript";
    __map_ext_mime[".json"] = "application/json";
    __map_ext_mime[".jsonld"] = "application/ld+json";
    __map_ext_mime[".mid"] = "audio/midi";
    __map_ext_mime[".midi"] = "audio/x-midi";
    __map_ext_mime[".mjs"] = "text/javascript";
    __map_ext_mime[".mp3"] = "audio/mpeg";
    __map_ext_mime[".mp4"] = "video/mp4";
    __map_ext_mime[".mpeg"] = "video/mpeg";
    __map_ext_mime[".mpkg"] = "application/vnd.apple.installer+xml";
    __map_ext_mime[".odp"] = "application/vnd.oasis.opendocument.presentation";
    __map_ext_mime[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    __map_ext_mime[".odt"] = "application/vnd.oasis.opendocument.text";
    __map_ext_mime[".oga"] = "audio/ogg";
    __map_ext_mime[".ogv"] = "video/ogg";
    __map_ext_mime[".ogx"] = "application/ogg";
    __map_ext_mime[".opus"] = "audio/opus";
    __map_ext_mime[".otf"] = "font/otf";
    __map_ext_mime[".png"] = "image/png";
    __map_ext_mime[".pdf"] = "application/pdf";
    __map_ext_mime[".php"] = "application/x-httpd-php";
    __map_ext_mime[".ppt"] = "application/vnd.ms-powerpoint";
    __map_ext_mime[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    __map_ext_mime[".rar"] = "application/vnd.rar";
    __map_ext_mime[".rtf"] = "application/rtf";
    __map_ext_mime[".sh"] = "application/x-sh";
    __map_ext_mime[".svg"] = "image/svg+xml";
    __map_ext_mime[".tar"] = "application/x-tar";
    __map_ext_mime[".tif"] = "image/tiff";
    __map_ext_mime[".tiff"] = "image/tiff";
    __map_ext_mime[".ts"] = "video/mp2t";
    __map_ext_mime[".ttf"] = "font/ttf";
    __map_ext_mime[".txt"] = "text/plain";
    __map_ext_mime[".vsd"] = "application/vnd.visio";
    __map_ext_mime[".wav"] = "audio/wav";
    __map_ext_mime[".weba"] = "audio/webm";
    __map_ext_mime[".webm"] = "video/webm";
    __map_ext_mime[".webp"] = "image/webp";
    __map_ext_mime[".woff"] = "font/woff";
    __map_ext_mime[".woff2"] = "font/woff2";
    __map_ext_mime[".xhtml"] = "application/xhtml+xml";
    __map_ext_mime[".xls"] = "application/vnd.ms-excel";
    __map_ext_mime[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    __map_ext_mime[".xml"] = "application/xml";
    __map_ext_mime[".xul"] = "application/vnd.mozilla.xul+xml";
    __map_ext_mime[".zip"] = "application/zip";
    __map_ext_mime[".3gp"] = "video/3gpp";
    __map_ext_mime[".3g2"] = "video/3gpp2";
    __map_ext_mime[".7z"] = "application/x-7z-compressed";
    for (std::map<std::string, std::string>::const_iterator it = __map_ext_mime.begin(); it != __map_ext_mime.end(); ++it)
        __map_mime_ext.insert(std::make_pair(it->second, it->first));
    __map_mime_ext["audio/3gpp"] = ".3gp";
    __map_mime_ext["audio/3gpp2"] = ".3gp2";
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
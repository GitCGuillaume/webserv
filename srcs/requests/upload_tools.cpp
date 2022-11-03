#include "upload_tools.hpp"

/*  https://www.rfc-editor.org/rfc/rfc1867
        When the user completes the form, and selects the SUBMIT element, the
    browser should send the form data and the content of the selected
    files.  The encoding type application/x-www-form-urlencoded is
    inefficient for sending large quantities of binary data or text
    containing non-ASCII characters.  Thus, a new media type,
    multipart/form-data, is proposed as a way of efficiently sending the
    values associated with a filled-out form from client to server.
*/

void get_boundary(std::string const &content_type, std::string &boundary, bool *boundary_ok)
{
    size_t i = 0;
    size_t start_boundary = content_type.find("boundary=");

    boundary = content_type.substr(start_boundary + 9, content_type.size());
    if (start_boundary == std::string::npos)
    {
        *boundary_ok = false;
        return;
    }
    if (boundary[0] == '\"' && boundary[boundary.size() - 1] == '\"') // check if "" then remove
    {
        boundary.erase(0, 1);
        boundary.erase(boundary.size() - 1, 1);
        while (boundary[i] && boundary[i] != '\"')
            ++i;
        if (boundary[i] && boundary[i] == '\"')
        {
            *boundary_ok = false;
            return;
        }
    }
    if (boundary.size() > 70)
    {
        *boundary_ok = false;
        return;
    }
    std::cout << "Boundary : |" << boundary << "|" << std::endl;
    *boundary_ok = true;
}

void get_media_type(std::string const &content_type, std::string &type,
                    std::string &sub_type)
{
    size_t find_slash = content_type.find("/");
    size_t find_comma = content_type.find(";");

    if (find_slash == std::string::npos || find_comma == std::string::npos)
        return;
    std::cout << "content_type:" << content_type << std::endl;
    type = content_type.substr(0, find_slash);
    sub_type = content_type.substr(find_slash, find_comma - find_slash);
}

// gchopin.42.fr/upload/
bool use_cgi(std::string const url, std::string const type_cgi)
{
    size_t i = url.size();
    std::string str("a");

    if (!type_cgi.size())
        return (false);
    while (0 < i)
    {
        if (url[i] == '.')
        {
            str = url.substr(i, type_cgi.size());
            break;
        }
        --i;
    }
    if (str.compare(type_cgi) == 0)
        return (true);
    return (false);
}

void upload_file(std::string const &body, std::string const &boundary)
{
    std::string filename;
    std::string copy_boundary;
    size_t pos = 0;
    size_t pos_old = 0;

    // boucle
    // check boundary

    // find first \r\n
    if (body[pos] && body[pos] == '-' && body[pos + 1] && body[pos + 1] == '-')
    {
        pos += 2;
        pos_old = pos;
        pos = body.find("\r\n");
        copy_boundary = body.substr(pos_old, pos - pos_old);
        if (copy_boundary.compare(boundary) != 0)
        {
            // err
            return;
        }
        copy_boundary.clear();
        pos = body.find("filename=\"");
        pos_old = pos + 10;
        pos = body.find("\"", pos_old);
        filename = body.substr(pos_old, pos - pos_old);
        std::cout << "filename: |" << filename << "|" << std::endl;
        pos = body.find("\r\n\r\n");
        pos_old = pos;
        std::cout << "pos_old: " << body[pos_old] << " pos: " << body[pos + 4] << std::endl;
    }
}

bool try_upload(std::string const content_type, std::string const body, long const content_lenght)
{
    std::string type;
    std::string sub_type;
    std::string boundary;
    bool boundary_ok = false;

    get_media_type(content_type, type, sub_type);
    if (type.size() == 0 || sub_type.size() == 0 || type.compare("multipart") != 0 || sub_type.compare("/form-data") != 0)
        return (false);
    std::cout << "type:" << type << " subtype:" << sub_type << std::endl;
    get_boundary(content_type, boundary, &boundary_ok);
    if (boundary_ok == false)
        return (boundary_ok);
    upload_file(body, boundary);
    return (true);
}
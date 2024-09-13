#include "Response.hpp"
#include "Server.hpp"
#include <string.h>

Response::Response(Request &request): request(&request)
{

}

Response::Response()
{

}

Response::~Response()
{

}

void Response::GetPath(Server &server)
{
    std::vector<std::string>                    routes;
    size_t                                      pos = 0;


    while (++pos <= request->uri.length())
    {
        routes.push_back(request->uri.substr(0, request->uri.find('/', pos) + 1));
        pos = request->uri.find('/', pos);
        if (pos == std::string::npos)
        {
            routes.push_back(request->uri + '/');
            break;
        }
    }

    std::map<std::string, Location>::iterator   it = server.getAllLocations().begin();
    std::vector<std::string>::iterator it1 = routes.begin();


    for(; it1 != routes.end(); it1++)
    {
        if(server.getAllLocations().find(*it1) != server.getAllLocations().end())
        {    
            path = server.getAllLocations().find(*it1)->second.root;
            path += request->uri.erase(0, it1->length());
            break;
        }
    }
    std::cout << path << std::endl;
}

std::string Response::getContentLength()
{
    std::string header;

    struct stat st;

    stat(path.c_str(), &st);

    header = "Content-Lentgh: " + std::to_string(st.st_size) + "\r\n";
    return (header);
}

std::string Response::getContentType(const char* path)
{
    const char *last_dot = strrchr(path, '.');
    
    if (last_dot) 
    {
        if (strcmp(last_dot, ".css") == 0) return "Content-type: text/css\r\n";
        if (strcmp(last_dot, ".csv") == 0) return "Content-type: text/csv\r\n";
        if (strcmp(last_dot, ".gif") == 0) return "Content-type: image/gif\r\n";
        if (strcmp(last_dot, ".htm") == 0) return "Content-type: text/html\r\n";
        if (strcmp(last_dot, ".html") == 0) return "Content-type: text/html\r\n";
        if (strcmp(last_dot, ".ico") == 0) return "Content-type: image/x-icon\r\n";
        if (strcmp(last_dot, ".jpeg") == 0) return "Content-type: image/jpeg\r\n";
        if (strcmp(last_dot, ".jpg") == 0) return "Content-type: image/jpeg\r\n";
        if (strcmp(last_dot, ".js") == 0) return "Content-type: application/javascript\r\n";
        if (strcmp(last_dot, ".json") == 0) return "Content-type: application/json\r\n";
        if (strcmp(last_dot, ".png") == 0) return "Content-type: image/png\r\n";
        if (strcmp(last_dot, ".pdf") == 0) return "Content-type: application/pdf\r\n";
        if (strcmp(last_dot, ".svg") == 0) return "Content-type: image/svg+xml\r\n";
        if (strcmp(last_dot, ".txt") == 0) return "Content-type: text/plain\r\n";
        if (strcmp(last_dot, ".mp4") == 0) return "Content-type: video/mp4\r\n";
    }

    return "Content-type: application/octet-stream\r\n";
}

std::string Response::getBody()
{
    char buffer[1025];
    
    bzero(buffer, 1025);
    file.read(buffer, 1024);

    std::cout << buffer << std::endl;
    if (file.eof())
        file.close();
    return (buffer);
}

bool Response::Serving(int clientFd, Server &server)
{

    if (!request->uri.compare("GET"))
        return GetRespond(clientFd, server);
    else if (!request->uri.compare("POST"))
        return PostRespond(clientFd, server);
    else if (!request->uri.compare("DELETE"))
        return DeleteRespond(clientFd, server);        

}

bool Response::GetRespond(int &clientFd, Server &server)
{
    if (!file.is_open())
    {
        std::string respondHeaders;

        GetPath(server);

        // if (access(path.c_str(), ))
        file.open(path);

        respondHeaders += "HTTP/1.1 200 OK\r\n";
        respondHeaders += getContentType(path.c_str());
        respondHeaders += getContentLength();
        respondHeaders += "\r\n";

        respondHeaders += getBody();

        send(clientFd, respondHeaders.c_str(), respondHeaders.length(), 0);  
        if (!file.is_open())
            return true;
        return false;
    }
    else
    {
        std::string body = getBody();

        if (!file.eof())
        {                
            send(clientFd, body.c_str(), body.length(), 0);
            return false;
        }
        else
        {
            file.close();
            return true;
        }
    }
}

bool Response::PostRespond(int &clientFd, Server &server)
{
    return (true);
}

bool Response::DeleteRespond(int &clientFd, Server &server)
{
    return (true);
}
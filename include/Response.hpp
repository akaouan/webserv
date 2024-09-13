#ifndef _RESPONSE_
# define _RESPONSE_

#include "Request.hpp"
#include <sys/socket.h>
#include <fstream>

class Server;

class Response
{
    private:
        Request         *request;
        std::string     path;
        std::ifstream   file;
    
    public:
        Response();
        Response(Request &request);
        ~Response();

        std::string getContentType(const char*);
        std::string getContentLength();
        std::string getBody();

        void GetPath(Server &server);

        bool Serving(int, Server&);

        bool GetRespond(int &, Server &);
        bool PostRespond(int &, Server &);
        bool DeleteRespond(int &, Server &);

};

#endif
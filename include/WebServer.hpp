#ifndef _WEB_SERVER_
# define _WEB_SERVER_

#include "Config.hpp"
#include "Server.hpp"
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>

#define MAX_FD      1024
#define BUFFER_SIZE 1024

class WebServer
{
    public:
        WebServer(const Config &config);
        WebServer();

        Config                          config;
        size_t                          max_fd;
        std::pair<const int, Client>    *client;
		Request							*request;

    public:
        void    Build();
        void    Run();

    private:
        void                        setSockets(fd_set*, fd_set*);
        void                        AcceptNewConnection(fd_set*);
        void                        createSocket(Server &server);
        void                        HandleRequest(fd_set*, fd_set*);
        void                        dropClient(const int&);
        void                        sendTheRest(fd_set*);
};

#endif
#include "WebServer.hpp"

WebServer::WebServer(const Config &config){
	this->config = config;
}

WebServer::WebServer(): client(NULL)
{
	Location *loc;
    config.server.push_back(Server());
    config.server[0].setListen("9090");
    config.server[0].setHost("127.0.0.1");
    config.server[0].setBodySize(1024);
    config.server[0].setErrorPage(int(), std::string("/home/kali/Desktop)"));
    config.server[0].addLocation(std::string("/dir/"), Location());

  	loc = &config.server[0].getLocations("/dir/");

    loc->auto_index = 1;
    loc->cgi_pass.push_back(std::make_pair(std::string("php"), std::string("/home/kali/Desktop/")));
    loc->upload_pass = "/home/kali/Desktop/";
    loc->index = "/home/kali/Desktop/";
    loc->root = "/home/kali/Desktop/";
    
}

void  WebServer::createSocket(Server &server)
{
    struct addrinfo hints;
    struct addrinfo *bind_address;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(server.getHost().c_str(), server.getListen().c_str(), &hints, &bind_address);

    int socket_listen;

    socket_listen = socket(bind_address->ai_family,
    bind_address->ai_socktype, bind_address->ai_protocol);
    
    if (socket_listen == -1) {
        std::cout <<  "[-] socket(): " << strerror(errno) << std::endl;
        exit(1);
    }
    
    int yes = 1;
    setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    
    if (bind(socket_listen,
        bind_address->ai_addr, bind_address->ai_addrlen)) {
    
        std::cout << "[-] bind() " << strerror(errno) << std::endl;
        exit(1);
    }
    freeaddrinfo(bind_address);
    
    if (listen(socket_listen, MAX_FD) < 0) {
        std::cout << "[-] listen(): " << strerror(errno) << std::endl;
        exit(1);
    }
    server.socket = socket_listen;
}

void WebServer::Build()
{
    std::vector<Server>::iterator it = config.server.begin();
    for(;it != config.server.end(); it++)
    {
        createSocket(*it);
    }
}

void WebServer::Run()
{
    while (1)
    {
        fd_set          readFds;
        fd_set          writeFds;
        struct timeval  timeout;
        
        FD_ZERO(&readFds);
        FD_ZERO(&writeFds);
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 1;
        client = NULL;
        setSockets(&readFds, &writeFds);

        if (select(max_fd + 1, &readFds, &writeFds, NULL, &timeout) == -1)
            std::cout << "[-] select(): " << strerror(errno) << std::endl;        

        AcceptNewConnection(&readFds);

        HandleRequest(&readFds, &writeFds);

    }
}

void WebServer::HandleRequest(fd_set *readFds, fd_set *writeFds)
{
    std::vector<Server>::iterator                           it = config.server.begin();
    std::vector<int>                                        dropedClients;
    
    for(;it != config.server.end(); it++)
    {
        std::map<int, Client>::iterator it1 = it->client.begin();
        for(;it1 != it->client.end(); it1++)
        {

            if (FD_ISSET(it1->first, readFds))
            {
                // Request Parse
                char buffer[BUFFER_SIZE + 1];
                
                bzero(buffer, BUFFER_SIZE + 1);
                int recieved = recv(it1->first, buffer, BUFFER_SIZE, 0);
                if (recieved > 0)
                {
				    it1->second.request.requestParser(buffer);
                }
            }
            else if (FD_ISSET(it1->first, writeFds))
            {
                // Response
                if (it1->second.response.Serving(it1->first, *it))
                    dropedClients.push_back(it1->first);
            }
        }
    }

    std::vector<int>::iterator itr = dropedClients.begin();

    for(;itr != dropedClients.end(); itr++)
        dropClient(*itr);

}

void WebServer::dropClient(const int &clientFd)
{
    std::vector<Server>::iterator it = config.server.begin();
    close(clientFd);
    for(;it != config.server.end(); it++)
        it->client.erase(clientFd);
}

void WebServer::setSockets(fd_set *readFds, fd_set *writeFds)
{
    std::vector<Server>::iterator it = config.server.begin();

    for(;it != config.server.end(); it++)
    {
        FD_SET(it->socket, readFds);
        FD_SET(it->socket, writeFds);
        max_fd = it->socket;
        std::map<int, Client>::iterator it1 = it->client.begin();
        for(;it1 != it->client.end(); it1++)
        {
            FD_SET(it1->first, readFds);
            FD_SET(it1->first, writeFds);
            if (it1->first > max_fd)
                max_fd = it1->first;
        }
    } 
	
}

void    WebServer::AcceptNewConnection(fd_set *readFds)
{
    std::vector<Server>::iterator it = config.server.begin();

    sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    for(;it != config.server.end(); it++)
    {
        if (FD_ISSET(it->socket, readFds))
        {
            int client_fd;

            client_fd = accept(it->socket, (struct sockaddr*)&clientAddress, (socklen_t *)&clientAddressLength);
            if (client_fd == -1)
            {
                std::cout << "[-] accpet(): " << strerror(errno) << std::endl;
                exit(1);
            }
            std::cout << "[+] New connection from: " << inet_ntoa(clientAddress.sin_addr) << ", Client socket: " << client_fd << ", Server Socket: " << it->socket << std::endl;

            it->client.insert(std::make_pair(client_fd, Client()));
            return ;
        }
    }
}

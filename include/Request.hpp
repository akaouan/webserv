#ifndef _REQUEST_
# define _REQUEST_

#include <map>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>

class Request
{
    private:
        bool                                nflag;
    public:
        std::string                         method;
        std::string                         uri;
		std::string							version;
        std::map<std::string, std::string>  headers;
		int									status_code;

    public:
        Request(/* args */);
        Request(const Request &other);
        ~Request();

    public:
		void requestParser(char* buffer);
		void firstLineParser(std::string line);
		int checkMethod(std::string method);
		int checkVersion(std::string version);
		int checkUri(std::string uri);

};

#endif
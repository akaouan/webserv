#include "Request.hpp"

Request::Request(): nflag(false) {}

Request::~Request() {}

int	Request::checkMethod(std::string method)
{
	if (method == "GET" || method == "POST" || method == "DELETE")
		return (1);
	else
		return (0);
}

int	Request::checkVersion(std::string version)
{
	if (version == "HTTP/1.1" || version == "HTTP/1.0")
		return (1);
	else
		return (0);
}

int	Request::checkUri(std::string uri)
{
	std::string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~:/?#[]@!$&'()*+,;=%";
	size_t found = uri.find_first_not_of(allowed);
	if (found != std::string::npos)
		return (0);
	else
		return (1);
}

void Request::firstLineParser(std::string line)
{
	method = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	uri = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	version = line.substr(0, line.find('\r'));

	if(!checkMethod(method))
		status_code = 405;
	else if(!checkVersion(version))
		status_code = 505;
	else if (!checkUri(uri))
		status_code = 400;
}

void Request::requestParser(char* buffer)
{
	std::string requestMsg(buffer);
	std::string line;
	std::istringstream ss(requestMsg);
	std::getline(ss, line);

	firstLineParser(line);
}
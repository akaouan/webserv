#include "Config.hpp"

void	Config::ft_extention(int ac, char** av)
{
	if (ac > 2)
		throw exception_error("Wrong number of argument");

	char str[6] = "fnoc.";
	int i = 0;
	int j = 0;
	if (av[1])
	{
		while (av[1][i])
			i++;
		i--;
		while (str[j])
		{
			if (str[j++] != av[1][i--])
				throw exception_error("Extantion error");
		}
	}	
}

std::string	Config::collect_content(std::string file)
{
	std::fstream conf(file);
	
	if (!conf.is_open())
		throw exception_error("can not open the file");
	std::string line;
	std::string tmp;
	while (std::getline(conf, tmp))
		line = line + tmp + '\n';
	return(line);
}

void	Config::remove_comment(std::string &file)
{
	size_t	i;

	i = file.find("#");
	while(i != std::string::npos)
	{
		size_t	j = file.find("\n", i);
		file.erase(i, j - i);
		i = file.find("#");
	}
}

void	Config::remove_space(std::string &file)
{
	size_t	i = 0;

	while (file[i] && isspace(file[i]))
		i++;
	file = file.substr(i);
	i = file.length() - 1;
	while (i > 0 && isspace(file[i]))
		i--;
	file = file.substr(0, i + 1);
}

int Config::ft_strncomp(const std::string& str1, const std::string& str2, size_t count) 
{
    std::string substr1 = str1.substr(0, count);
    std::string substr2 = str2.substr(0, count);

    return substr1.compare(substr2);
}


void Config::split_servers(std::string &file)
{
	size_t start = 0;
	size_t end = 1;

	while (start != end && start < file.length())
	{
		start = findStartServer(start, file);
		end = findEndServer(start, file);
		if (start == end)
			throw exception_error("problem with scope");
		this->_str.push_back(file.substr(start, end - start + 1));
		this->num_of_server++;
		start = end + 1;
	}
}

size_t Config::findStartServer (size_t start, std::string &file)
{
	size_t i;

	for (i = start; file[i]; i++)
	{
		if (file[i] == 's')
			break ;
		if (!isspace(file[i]))
			throw  exception_error("Server did not find");
	}
	if (!file[i])
		return (start);
	if (file.compare(i, 6, "server") != 0)
		throw exception_error("Server did not find");
	i += 6;
	while (file[i] && isspace(file[i]))
		i++;
	if (file[i] == '{')
		return (i);
	else 
		throw  exception_error("problem with scope");

}

size_t Config::findEndServer (size_t start, std::string &file)
{
	size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start + 1; file[i]; i++)
	{
		if (file[i] == '{')
			scope++;
		if (file[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}

bool Config::isAllDigits(const std::string& str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        if (!isdigit(*it))
            return false;
    return true;
}

bool Config::isPathValid(const std::string& path) {
    if (access(path.c_str(), F_OK) != -1)
        return true;
    return false;
}

std::vector<std::string> Config::SplitFile(std::string& input)
{
    std::vector<std::string> lines;
    std::istringstream iss(input);
    std::string line;

    while (std::getline(iss, line))
    {
        std::string delimiter = " \t\n";
        size_t start = line.find_first_not_of(delimiter);
        size_t end = line.find_last_not_of(delimiter);

        if (start != std::string::npos && end != std::string::npos)
        {
            std::string trimmedLine = line.substr(start, end - start + 1);
            std::istringstream lineStream(trimmedLine);
            std::string word;
            while (lineStream >> word)
                lines.push_back(word);
        }
		// std::cout << "=> "<< line << std::endl;
    }
    return lines;
}


std::vector<std::pair<std::string, std::string> > Split_File(std::string& input) {
    std::vector<std::pair<std::string, std::string> > pairs;
    std::istringstream iss(input);
    std::string line;

    while (std::getline(iss, line)) {
        std::string delimiter = " \t\n";
        size_t start = line.find_first_not_of(delimiter);
        size_t end = line.find_last_not_of(delimiter);

        if (start != std::string::npos && end != std::string::npos) {
            std::string trimmedLine = line.substr(start, end - start + 1);
            std::istringstream lineStream(trimmedLine);
            std::string firstWord;
            lineStream >> firstWord;  // Extract the first word

            std::string restOfLine;
            std::getline(lineStream, restOfLine);  // Get the rest of the line

            pairs.push_back(std::make_pair(firstWord, restOfLine));  // Push the pair into the vector
        	// std::cout << "first : " << firstWord << " ======== second : " <<  restOfLine <<   std::endl;
        }
    }
    return pairs;
}

int Config::ft_stoi(std::string str)
{
    std::stringstream ss(str);
    if (str.length() > 10)
        throw exception_error("MAX Body Size");
    for (size_t i = 0; i < str.length(); ++i)
        if(!isdigit(str[i]))
            throw exception_error("Body Size");
    int res;
    ss >> res;
    return (res);
}

Config::Config(int ac, char **av)
{
	
	std::string file;
	std::string content;

	server.reserve(3);
	
	ft_extention(ac, av);
	if (ac == 2)
		file = av[1];
	else
		file = "conf/Default.conf";
	content = collect_content(file);
	remove_space(content);
	if (content.empty())
		throw exception_error("Empty file");
	remove_comment(content);
	split_servers(content);
	size_t j = this->num_of_server;
	Server serv;
	for(size_t i = 0; i < j; i++)
	{
		serv = ConfigurationServer(this->_str[i]);
		this->server.push_back(serv);
	}
		std::cout << "\n\n-------------------------------------------------------------------\n\n";
	for(int i = 0; i < j; i++)
	{
		std::cout << "lesten " << server[i].getListen() << std::endl;
		std::cout << "host " << server[i].getHost() << std::endl;
		std::cout << "ServerName " << server[i].getServerName() << std::endl;
		std::cout << "body size " << server[i].getBodySize() << std::endl;

		std::cout << "\n\n-------------------------------------------------------------------\n\n";
		std::map<int, std::string> erre ; 
		erre = server[i].getErrorPage();
		std::map<int, std::string>::iterator it = erre.begin(); 
		std::map<int, std::string>::iterator itt = erre.end(); 

		std::cout << "Error Pages\n";
		while (it != itt)
		{ 
			std::cout << it->first << " :: " << it->second << std::endl; 
			it++; 
		}

		std::map<std::string, Location>	 lo ;
		lo = server[i].getAllLocations();
		std::map<std::string, Location>::iterator it2 = lo.begin();
		std::map<std::string, Location>::iterator itt2 = lo.end();

		while (it2 != itt2)
		{
			std::vector<std::pair <std::string, std::string> > cgi = it2->second.cgi_pass;
			std::vector<std::pair <std::string, std::string> >::iterator cgi_it = cgi.begin();
			std::vector<std::pair <std::string, std::string> >::iterator cgi_itt = cgi.end();

			std::cout << "\n\n*****************************\n\n";
			std::cout << "location path : " << it2->first << " \n" << "auto_index : " << it2->second.auto_index << "\n" \
				 << "methods : " << it2->second.methods << "\n"  << "root : " << it2->second.root << "\n" \
				  << "upload_pass : " << it2->second.upload_pass << "\n" << std::endl;
			while (cgi_it != cgi_itt)
			{
				std::cout << "cgi : " << cgi_it->first << " => " << cgi_it->second << "\n";
				cgi_it++;
			}
			it2++;
		}
	}
}

std::string RemoveSpaces(std::string& input) {
    std::string result;

    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] != ' ' && input[i] != '\t') {
            result += input[i];
        }
    }

    return result;
}

int Config::setMethods(std::vector<std::string> method) 
{
	int	methods = 0;
	try{
		std::set<std::string> validMethod;
		validMethod.insert("GET");
		validMethod.insert("POST");
		validMethod.insert("DELETE");
		for (int i = 0; i < method.size(); i++)
		{
			if (!validMethod.count(method[i]))
				throw exception_error("'" + method[i] +"` Invalid method");
			methods += method[i].size();
			//explain the concept
			//if GET mean = 3
			//if POST mean = 4
			//if DELETE mean = 6
			//if GET + POST = 7
			//if GET + DELETE = 9
			//if POST + DELETE = 10
			//if GET + POST + DELETE = 13
		}
	}
	catch(const std::exception& e) {std::cerr << e.what() << std::endl;}
	return methods;
}

std::vector<std::pair <std::string, std::string> > CgiPath(std::string str)
{
	std::vector<std::pair <std::string, std::string> > cgi_;
	std::istringstream iss(str);
    std::string word;
   	iss >> word;
    std::string::size_type pos = word.find(' ');
    std::string key, value;
    key = word.substr(0, pos);
    value = word.substr(pos + 1);
	std::cout << "key : " << key <<std::endl;
	std::cout << "value : " << value <<std::endl;
	// cgi_.push_back(std::make_pair(key, value));
	return cgi_;
}

Location Config::setpartlocation(std::vector<std::pair<std::string, std::string> > &content)
{
	Location loc;
	for (size_t i = 0; i < content.size(); i++) 
	{
		if (content[i].first == "allow_methods" && i + 1 < content.size())
		{
			std::vector<std::string> _methods;
			std::istringstream iss(content[i].second);
   			std::string word;
   			while (iss >> word) {
        		_methods.push_back(word);
    		}
			loc.methods = setMethods(_methods);
		}
		else if (content[i].first == "index" && i + 1 < content.size())
		{
			loc.index = content[i].second;
		}
		else if (content[i].first == "root" && i + 1 < content.size())
		{
			loc.root = content[i].second;
		}
		else if (content[i].first == "autoindex" && i + 1 < content.size())
		{
			content[i].second = RemoveSpaces(content[i].second);
			if (content[i].second == "1")
				loc.auto_index = true;
			else if (content[i].second == "0")
				loc.auto_index = false;
			else
				loc.auto_index = false;
		}
		else if (content[i].first == "cgi_pass" && i + 1 < content.size())
		{
			loc.cgi_pass = CgiPath(content[i].second);
		}
		else if (content[i].first == "upload_pass" && i + 1 < content.size())
		{
			loc.upload_pass = content[i].second;
		}
    }
	return(loc);
}

Server	Config::ConfigurationServer(std::string& conf)
{
	Server serv;
	std::istringstream iss(conf);
	std::string line;
	std::vector<std::string> ptr;
	std::vector<std::pair<std::string, std::string> > tmp;

	ptr = SplitFile(conf);
	tmp = Split_File(conf);
	if (ptr.size() < 3)
		throw exception_error("Server Not Found");
	for(int j = 0; j < tmp.size(); j++)
	{
		if (tmp[j].first == "host" && (j + 1) < tmp.size())
		{
			serv.setHost(RemoveSpaces(tmp[j].second));
		}
		else if (tmp[j].first == "listen" && (j + 1) < tmp.size())
		{
			serv.setListen (RemoveSpaces(tmp[j].second));
		}
		else if (tmp[j].first == "body_size" && (j + 1) < tmp.size())
		{
			serv.setBodySize(ft_stoi(RemoveSpaces(tmp[j].second)));
		}
		else if (tmp[j].first == "server_name" && (j + 1) < tmp.size())
		{
			serv.setServerName(RemoveSpaces(tmp[j].second));
		}
		else if (tmp[j].first == "error_page" && (j + 1) < tmp.size())
		{
			j++;
			while (tmp[++j].first != "}")
			{
				if (!isAllDigits(tmp[j].first))
					throw exception_error(tmp[j].first + " is not valide");
				int errorCode;
        		errorCode = ft_stoi(tmp[j].first);
				// if (!isPathValid(tmp[j].second)) // 7ta itsetaw paths;
				// 	throw exception_error (tmp[j].second + " path invalid");
				std::pair<int, std::string> x = std::make_pair(errorCode, tmp[j].second);
				serv.setErrorPage(errorCode, tmp[j].second);
			}
		}
		else if (tmp[j].first == "location" && (j + 1) < tmp.size())
		{
			std::string path;
			Location loc;
			if (tmp[j].first == "{" || tmp[j].first == "}")
				throw exception_error("Location Scope error");
			path = tmp[j].second;
			if (tmp[++j].first != "{")
				throw exception_error("Scope error");
			j++;
			std::vector<std::pair<std::string, std::string> > LocatioCnontent;
			while(j < tmp.size() && tmp[j].first != "}")
			{
				LocatioCnontent.push_back(tmp[j]);
				j++;
			}
			loc = setpartlocation(LocatioCnontent);
			serv.addLocation(path, loc);
		}
		// std::cout << tmp[j].first << " " << tmp[j].second << std::endl;
	}
	// for (int i = 0; i < ptr.size(); i++)
	// {
	// 	if (ptr[i] == "host" && (i + 1) < ptr.size())
	// 	{
	// 		serv.setHost(ptr[++i]);
	// 	}
	// 	else if (ptr[i] == "listen" && (i + 1) < ptr.size())
	// 	{
	// 		serv.setListen (ptr[++i]);
	// 	}
	// 	else if (ptr[i] == "body_size" && (i + 1) < ptr.size())
	// 	{
	// 		serv.setBodySize(ft_stoi(ptr[++i]));
	// 	}
	// 	else if (ptr[i] == "server_name" && (i + 1) < ptr.size())
	// 	{
	// 		serv.setServerName(ptr[++i]);
	// 	}
	// 	else if (ptr[i] == "error_page" && (i + 1) < ptr.size())
	// 	{
	// 		if (ptr[++i] != "{")
	// 			throw exception_error("Scope error");
	// 		while (ptr[++i] != "}")
	// 		{
	// 			if (!isAllDigits(ptr[i]))
	// 				throw exception_error(ptr[i] + " is not valide");
	// 			int errorCode;
    //     		errorCode = std::stoi(ptr[i]);
	// 			// if (!isPathValid(ptr[i + 1])) // 7ta itsetaw paths;
	// 			// 	throw exception_error (ptr[i + 1] + " path invalid");
	// 			std::pair<int, std::string> x = std::make_pair(errorCode, ptr[i + 1]);
	// 			serv.setErrorPage(errorCode, ptr[i + 1]);
	// 			i++;
	// 		}
	// 	}
	// 	else if (ptr[i] == "location" && (i + 1) < ptr.size())
	// 	{
	// 		std::string path;
	// 		Loca loc;
	// 		i++;
	// 		if (ptr[i] == "{" || ptr[i] == "}")
	// 			throw exception_error("Location Scope error");
	// 		path = ptr[i];
	// 		if (ptr[++i] != "{")
	// 			throw exception_error("Scope error");
	// 		i++;
	// 		std::vector<std::string> LocatioCnontent;
	// 		while(i < ptr.size() && ptr[i] != "}")
	// 			LocatioCnontent.push_back(ptr[i++]);
	// 		loc = setpartlocation(LocatioCnontent);
	// 		serv.addLocation(path, loc);
	// 	}
	// }
	return serv;
}

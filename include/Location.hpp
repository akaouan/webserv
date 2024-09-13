// #ifndef _LOCATION_
// # define _LOCATION_

// #include <string>
// #include "Client.hpp"
// #include <vector>
// #include <string>
// #include <set>
// #include <map>

// class Location
// {
// 	private :
// 		int											methods;
//     	bool	 									auto_index;
//     	std::string									index;
//     	std::string									root;
//     	std::string									upload_pass;
//     	std::map<std::string, std::string>			cgi_pass;

// 	public :
// 		Location():methods(0), auto_index(false) {};
// 		~Location() {};
// 		Location(const Location& obj): methods(obj.methods), index(obj.index), auto_index(obj.auto_index), root(obj.root), upload_pass(obj.upload_pass), cgi_pass(obj.cgi_pass) {};
		
// 		Location operator=(const Location& obj)
// 		{
// 			this->methods = obj.methods;
// 			this->index = obj.index;
// 			this->auto_index = obj.auto_index;
// 			this->root = obj.root;
// 			this->upload_pass = obj.upload_pass;
// 			this->cgi_pass = obj.cgi_pass;
		
// 			return *this;
// 		}
		
// 		int  setMethods		(std::vector<std::string> meth);
// 		void setIndex		(std::string &index);
// 		void setAutoIndex	(bool auto_index);
// 		void setRoot		(std::string &root);
// 		void setUploadPass	(std::string &upload_pass);
// 		void setCgiPass		(std::map<std::string, std::string> cgi_pass);

// 		int 									getMethods		();
// 		bool 									getAutoIndex	();
// 		std::string 							getIndex		();
// 		std::string 							getRoot			();
// 		std::string 							getUploadPass	();
// 		std::map<std::string, std::string> 		getCgiPass		();
		

// 		class LocationError : public std::exception {
// 			private :
// 				std::string Msg;		

// 			public :

// 				LocationError(std::string message) throw(){
// 					Msg = "Error: " + message;
// 				}

// 				virtual const char* what() const throw(){
// 					return (Msg.c_str());
// 				};
// 				virtual ~LocationError() throw() {}
// 		};
// };

// #endif
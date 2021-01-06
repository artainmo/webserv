#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <list>
 
#define P(x) std::cout << x << std::endl
#define pass (void)0

typedef struct s_CGI
{
	std::string AUTH_TYPE;
	std::string CONTENT_LENGTH;
	std::string CONTENT_TYPE;
	std::string GATEWAY_INTERFACE;
	std::string PATH_INFO;
	std::string PATH_TRANSLATED;
	std::string QUERY_STRING;
	std::string REMOTE_ADDR;
	std::string REMOTE_INDENT;
	std::string REMOTE_USER;
	std::string REQUEST_METHOD;
	std::string REQUEST_URI;
	std::string SCRIPT_NAME;
	std::string SERVER_NAME;
	std::string SERVER_PORT;
	std::string SERVER_PROTOCOL;
	std::string SERVER_SOFTWARE;
} t_CGI;

typedef struct s_location //Config routes
{
	std::list<std::string> file_extensions; //if  no extensions specified means ALL, besides if extension specified in other location
	std::string link_extension;
	std::list<std::string> http_methods;
	std::string root;
	std::string  directory_listing;
	std::string default_file_if_request_directory;
	t_CGI *CGI; //If equal to NULL no CGI server, but http static content server
	std::string file_upload_location;
} t_location;

typedef struct s_config
{
	std::string host;
	std::string port;
	std::string server_name;
	std::string default_error_page;
	std::string body_size_limit;
	std::list<t_location> locations;
} t_config;

t_config *parse_config(std::string path);
bool getlinecut(std::ifstream &fd, std::string &line);
bool check_line(std::string line, const std::string &comp);
std::string following_content(std::string line, const std::string &after);
std::string parse_between(std::string &line, char cut, char cut2, bool between=true);
std::string parse_until(std::string &line, char until, bool all=false);
void show_conf(t_config &conf);


#endif

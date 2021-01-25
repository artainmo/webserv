#ifndef PARSING_HPP
#define PARSING_HPP

#include "../utils/utils.hpp"

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
	std::string directory;
	std::list<std::string> http_methods;
	std::string root;
	std::list<std::string> index;
	std::string  directory_listing;
	std::string default_file_if_request_directory;
	t_CGI *CGI; //If equal to NULL no CGI server, but http static content server
	std::string file_upload_location;
} t_location;

typedef struct s_config
{
	std::string host;
	int port;
	std::string server_name;
	std::string root;
	std::string default_error_page;
	int body_size_limit;
	std::list<t_location> locations;
	std::list<std::string> index;
} t_config;

typedef struct s_header_fields
{
	std::list<std::string> Accept_Charsets;
	std::list<std::string> Accept_Language;
	std::list<std::string> Allow;
	std::list<std::string> Authorization;
	std::list<std::string> Content_Language;
	std::list<std::string> Content_Length;
	std::list<std::string> Content_Location;
	std::list<std::string> Content_Type;
	std::list<std::string> Date;
	std::list<std::string> Host;
	std::list<std::string> Last_Modified;
	std::list<std::string> Location;
	std::list<std::string> Referer;
	std::list<std::string> Retry_After;
	std::list<std::string> Server;
	std::list<std::string> Transfer_Encoding;
	std::list<std::string> User_Agent;
	std::list<std::string> WWW_Authenticate;
} t_header_fields;

typedef struct s_http_req
{
	std::string method;
	std::string URL;
	t_location *loc;
	std::string protocol_version;
	t_header_fields header_fields;
	std::string message_body;
} t_http_req;

t_config *parse_config(std::string path);
t_http_req parse_http_request(std::string req, t_config &conf);
bool getlinecut(std::ifstream &fd, std::string &line);
bool check_line(std::string line, const std::string &comp);
std::string following_content(std::string line, const std::string &after);
std::list<std::string> following_contents(std::string line, const std::string &after);
std::list<std::string> split(std::string text, std::string sp);
std::string parse_between(std::string &line, char cut, char cut2, bool between=true);
std::string parse_until(std::string &line, std::string until, bool all=false);
void show_conf(t_config &conf);
void show_http_request(t_http_req &req);

#endif

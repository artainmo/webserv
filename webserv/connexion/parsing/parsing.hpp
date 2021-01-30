#ifndef PARSING_HPP
#define PARSING_HPP

#include "../utils/utils.hpp"

#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string.h>//strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>//close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <list>

#define P(x) std::cout << x << std::endl
#define pass (void)0

class internal_server_error_exc : public std::exception
{
public:
	const char *what() const _NOEXCEPT;
};

typedef struct s_CGI
{
	bool active;
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
	std::string SECRET;
} t_CGI;

typedef struct s_location //Config routes
{
	bool active;
	std::list<std::string> file_extensions; //if  no extensions specified means ALL, besides if extension specified in other location
	std::string directory;
	std::list<std::string> http_methods;
	int max_body;
	std::string root;
	std::list<std::string> index;
	std::string  directory_listing;
	std::string default_file_if_request_directory;
	t_CGI CGI; //If equal to NULL no CGI server, but http static content server
	std::string file_upload_location;
	std::string FOUND_URL;
} t_location;


typedef struct s_header_fields
{
	std::list<std::string> Accept_Charsets;
	std::list<std::string> Accept_Language;
	std::list<std::string> Allow;
	std::list<std::string> Authorization;
	std::list<std::string> Body;
	std::list<std::string> Content_Language;
	std::list<std::string> Content_Length;
	std::list<std::string> Content_Location;
	std::list<std::string> Content_Type;
	std::list<std::string> Date;
	std::list<std::string> Header_Line;
	std::list<std::string> Host;
	std::list<std::string> Last_Modified;
	std::list<std::string> Location;
	std::list<std::string> Referer;
	std::list<std::string> Retry_After;
	std::list<std::string> Server;
	std::list<std::string> Transfer_Encoding;
	std::list<std::string> User_Agent;
   	std::list<std::string> WWW_Authenticate;
	std::list<std::string> X_Secret;
} t_header_fields;

typedef struct s_http_req
{
	std::string complete_request;
	std::string method;
	std::string URL;
	t_location loc;
	std::string protocol_version;
	t_header_fields header_fields;
	std::string message_body;
	bool		error;
	bool		ready;
	bool		non_body_parsed;
	int			body_index;
	int			status_code;
} t_http_req;

typedef struct	s_server
{
	int server_socket; //Our server socket, whereby clients can connect to
	struct sockaddr_in address; //Socket address struct for socket functions
	int addrlen; //Size in int that can be casted for socket functions
	int client_socket[SOMAXCONN]; //Remember already connected clients
	int connected_socket; //New socket connected between server and client
	std::map<int, t_http_req> requests; //To handle multiple incoming requests and requests that are received in packages
	std::map<int, std::string>	answer; //To handle answers sent in packages
	unsigned int		fd_max;
}				t_server;

typedef struct s_config
{
	std::string host;
	std::list<std::string> port;
	std::string server_name;
	std::string root;
	std::string default_error_page;
	size_t body_size_limit;
	std::list<t_location> locations;
	std::list<std::string> index;
	t_server s;
} t_config;

void parse_config(std::string path, std::list<t_config> &ret);
void parse_http_request(t_http_req &ret, std::string &req, t_config &conf);
bool getlinecut(std::ifstream &fd, std::string &line);
bool check_line(std::string const &line, const std::string &comp);
std::string following_content(std::string &line, const std::string &after);
std::list<std::string> following_contents(std::string line, const std::string &after);
std::list<std::string> split(std::string text, std::string const &sp);
std::string parse_between(std::string &line, char cut, char cut2, bool between=true);
std::string parse_until(std::string &line, std::string const &until, bool all=false);
void show_conf(t_config &conf);
void show_locations(std::list<t_location> &locations);
void show_http_request(t_http_req &req);

void URL_to_local_path(t_http_req &req, t_config &conf);

#endif

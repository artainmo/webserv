#	ifndef CONNEXION_HPP
#	define CONNEXION_HPP

#include "utils/utils.hpp"
#include "parsing/parsing.hpp"

#include <map>
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

typedef struct	s_answer_headers
{
	std::string accept_charsets;
	std::string accept_language;
	std::string allow;
	std::string authorization;
	std::string content_language;
	std::string content_length;
	std::string content_location;
	std::string content_type;
	std::string date;
	std::string host;
	std::string last_modified;
	std::string location;
	std::string referer;
	std::string retry_after;
	std::string server;
	std::string transfer_encoding;
	std::string user_agent;
	std::string www_athenticate;
	std::string header_line;
	std::string body;
}				t_answer_headers;

// typedef struct s_socket
// {
// 	std::string request;
// 	std::string answer;
// } t_socket;

typedef struct	s_server
{
	int server_socket; //Our server socket, whereby clients can connect to
	struct sockaddr_in address; //Socket address struct for socket functions
	int addrlen; //Size in int that can be casted for socket functions
	int client_socket[SOMAXCONN]; //Remember already connected clients
	// std::map<int, t_socket> socket
	std::map<int, std::string> socket_to_answer;
	fd_set active_socket_read; //fd_set struct for select function, takes active sockets for reading
	fd_set active_socket_write;
	int connected_socket; //New socket connected between server and client
	std::map< int, std::string>	answer_to_send;
}				t_server;

void		setup_server(t_server &s, t_config &config);
void		wait_connexion(t_server &s, t_config &config);
void		new_incoming_connection(t_server &s, t_config &config);
void	send_answer(t_server &s, t_config &config);
void	get_client_request(t_server &s, t_config &config);

void		get_answer(std::map<int, std::string>::iterator &socket, t_http_req &req, t_config &conf, t_server &s);

std::string get_cgi(t_http_req &req);

#	endif

#	ifndef CONNEXION_HPP
#	define CONNEXION_HPP

#include "utils/utils.hpp"
#include "parsing/parsing.hpp"

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

typedef struct	s_server
{
	int server_socket; //Our server socket, whereby clients can connect to
	struct sockaddr_in address; //Socket address struct for socket functions
	int addrlen; //Size in int that can be casted for socket functions
	int client_socket[SOMAXCONN]; //Remember already connected clients
	fd_set active_socket_read; //fd_set struct for select function, takes active sockets for reading
	fd_set active_socket_write;
	int connected_socket; //New socket connected between server and client
	std::map<int, t_http_req> requests; //To handle multiple incoming requests and requests that are received in packages
	std::map<int, std::string>	answer; //To handle answers sent in packages
	int		fd_max;
}				t_server;

void		setup_server(t_server &s, t_config &config);
void		new_incoming_connection(t_server &s);
void	get_client_request(t_server &s);
void client_restart(t_server &s, unsigned int i);
void client_disconnection(t_server &s, unsigned int i);
void disconnect_all(t_server &s, t_config &config);
void reset_sockets(t_server &s);

void		get_answer(std::map<int, t_http_req>::iterator &socket, t_http_req &req, t_config &conf, t_server &s);

std::string get_cgi(t_http_req &req, t_config &conf, t_server &s);

#	endif

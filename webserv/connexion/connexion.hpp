#ifndef CONNEXION_HPP
#define CONNEXION_HPP

#include "parsing/parsing.hpp"

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

#define PORT 8888 //For testing

typedef struct s_server
{
  int server_socket; //Our server socket, whereby clients can connect to
  struct sockaddr_in address; //Socket address struct for socket functions
  int addrlen; //Size in int that can be casted for socket functions
  int client_socket[SOMAXCONN]; //Remember already connected clients
  fd_set active_socket_read; //fd_set struct for select function, takes active sockets for reading
  int connected_socket; //New socket connected between server and client
} t_server;

void setup_server(t_server &s);
void wait_connexion(t_server &s);
void new_incoming_connection(t_server &s);
std::string get_client_request(t_server &s);

void answer_http_request(t_server &s, t_http_req &req);

#endif

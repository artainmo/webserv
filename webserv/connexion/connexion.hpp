#ifndef CONNEXION_HPP
#define CONNEXION_HPP

#include "parsing/parsing.hpp"

#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 80
#define BUFSIZE 4096

typedef struct sockaddr_in SA_IN ;

std::string receive_http_request(t_config config);
void answer_http_request(t_http_req req);

....

#endif

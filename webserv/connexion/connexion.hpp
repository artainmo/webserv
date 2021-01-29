#	ifndef CONNEXION_HPP
#	define CONNEXION_HPP

#include "utils/utils.hpp"
#include "parsing/parsing.hpp"

typedef struct s_active_socket
{
  fd_set read; //fd_set struct for select function, takes active sockets for reading
  fd_set write;
} t_active_socket;

void		setup_server(t_config &config);
void		new_incoming_connection(t_server &s, t_active_socket &active_socket);
void	get_client_request(t_server &s, t_active_socket &active_socket);
void internal_server_error(t_server &s);
void client_restart(t_server &s, unsigned int i);
void client_restart_all(t_server &s);
void client_disconnection(t_server &s, unsigned int i);
void disconnect_all(t_server &s, t_active_socket &active_socket);
void all_servers_reset_sockets(std::list<t_config> &c, t_active_socket &active_socket);
std::string error_page(int error_nbr, std::string methode);
void all_servers(std::list<t_config> &c, t_active_socket &active_socket, void (*f)(t_server &, t_active_socket &));


void		get_answer(std::map<int, t_http_req>::iterator &socket, t_http_req &req, t_config &conf);

std::string get_cgi(t_http_req &req, t_config &conf);

#	endif

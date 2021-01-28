#	ifndef CONNEXION_HPP
#	define CONNEXION_HPP

#include "utils/utils.hpp"
#include "parsing/parsing.hpp"

void		setup_server(t_config &config);
void		new_incoming_connection(t_server &s);
void	get_client_request(t_server &s);
void internal_server_error(t_server &s);
void client_restart(t_server &s, unsigned int i);
void client_restart_all(t_server &s);
void client_disconnection(t_server &s, unsigned int i);
void disconnect_all(t_server &s);
void reset_sockets(t_server &s);
std::string error_page(int error_nbr, std::string methode);


void		get_answer(std::map<int, t_http_req>::iterator &socket, t_http_req &req, t_config &conf);

std::string get_cgi(t_http_req &req, t_config &conf);

#	endif

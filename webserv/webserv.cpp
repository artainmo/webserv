#include "connexion/connexion.hpp"

int main(int argc , char *argv[])
{
	t_server *s = new t_server;
	t_config *config;
	t_http_req *req;
	std::string request;

	if (argc != 2)
	{
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
		exit(1);
	}
	config = parse_config(argv[1]);
    setup_server(*s);
    while(true)
    {
        wait_connexion(*s);
        if (FD_ISSET(s->server_socket, &s->active_socket_read)) //If returns true, something happened on server socket, meaning a new connexion occured
        new_incoming_connection(*s);
        if ((request = get_client_request(*s)) != std::string("None"))
        {
					req = parse_http_request(request);
			// answer_http_request(req, conn);
        }
    }
    return 0;
}

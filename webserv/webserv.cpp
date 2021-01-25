#include "connexion/connexion.hpp"

void responded_socket(std::map<int, std::string>::iterator &socket, t_server &s)
{
	int rem;

	std::cout << "Erasing socket: "<< socket->first << std::endl;
	rem = socket->first;
	socket++;
	s.socket_to_answer.erase(rem);
}

void change_directory(std::string relative_path)
{
	char *path_name = new char[100];

	if (!(getcwd(path_name, 100)))
	{
		std::cout << "ERROR: getcwd failed" << std::endl;
		exit(1);
	}
	if (chdir((std::string(path_name) + relative_path).c_str()) == -1)
	{
		std::cout << "ERROR: chdir failed" << std::endl;
		exit(1);
	}
	// getcwd(path_name, 100);
	// std::cout << path_name << std::endl;
	delete [] path_name;
}

int main(int argc , char *argv[])
{
	t_server *s = new t_server;
	t_config *config;
	t_http_req *req;
	std::string request;
	std::map<int, std::string>::iterator socket;

	if (argc != 2)
	{
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
		exit(1);
	}
	signal(SIGPIPE, SIG_IGN); //Ignore closed pipe error - Client closes connextion when trying to send
	config = parse_config(argv[1]);
  setup_server(*s, *config);
  while(true)
  {
			// P("HE1");
      wait_connexion(*s, *config);
			// P("HE2");
      if (FD_ISSET(s->server_socket, &s->active_socket_read)) //If returns true, something happened on server socket, meaning a new connexion occured
      	new_incoming_connection(*s, *config);
			// P("HE3");
			change_directory("/frontend");
			// P("HE4");
			get_client_request(*s, *config);
	//		 P("HE5");
			// print_sockets(s->socket_to_answer);
			if (s->socket_to_answer.size() != 0)
      {
				socket = s->socket_to_answer.begin(); //socket is iterator map with socket fd (first) and request text (second)
				while(socket != s->socket_to_answer.end())
				{
						req = parse_http_request(socket->second, *config);
						if (answer_http_request(socket->first, *req, *config, *s) == true)
							responded_socket(socket, *s);
						else
							socket++;
				}
      }
			// print_sockets(s->socket_to_answer);
			change_directory("/..");
  }
  return 0;
}

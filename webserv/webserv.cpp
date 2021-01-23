#include "connexion/connexion.hpp"

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
      wait_connexion(*s, *config);
      if (FD_ISSET(s->server_socket, &s->active_socket_read)) //If returns true, something happened on server socket, meaning a new connexion occured
      	new_incoming_connection(*s, *config);
			change_directory("/frontend");
			get_client_request(*s, *config);
			if (s->socket_to_answer.size() != 0)
      {
				for (std::map<int, std::string>::iterator i = s->socket_to_answer.begin(); i != s->socket_to_answer.end(); i++)
				{
						req = parse_http_request(i->second, *config);
						answer_http_request(i->first, *req, *config, *s);
				}
      }
			change_directory("/..");
  }
  return 0;
}

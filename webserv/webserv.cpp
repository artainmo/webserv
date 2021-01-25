#include "connexion/connexion.hpp"


void server(t_server &s, t_config &conf)
{
	t_http_req *req;
	std::string request;
	std::map<int, std::string>::iterator socket;

	while(true)
  {
      wait_connexion(s, conf);
			send_answer(s, conf);
			get_client_request(s, conf);
			if (s.socket_to_answer.size() != 0)
      {
				socket = s.socket_to_answer.begin(); //socket is iterator map with socket fd (first) and request text (second)
				while(socket != s.socket_to_answer.end()) //If receiving multiple socket connections at same time handle them all
				{
						change_directory("/frontend");
						req = parse_http_request(socket->second, conf);
						if (req->ready == true)
							get_answer(socket, *req, conf, s);
						else
							socket++;
						change_directory("/..");
				}
      }
  }
}

int main(int argc , char *argv[])
{
	t_server *s = new t_server;
	t_config *config;

	if (argc != 2)
	{
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
		exit(1);
	}
	signal(SIGPIPE, SIG_IGN); //Ignore closed pipe error - Client closes connextion when trying to send
	config = parse_config(argv[1]);
  setup_server(*s, *config);
	server(*s, *config);
  return 0;
}

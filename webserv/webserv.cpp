#include "connexion/connexion.hpp"

void wait_connexion(t_server &s, t_config &config)
{
  int ret;
  struct timeval timeout = {10, 0};

  reset_sockets(s);
  if ((ret = select(FD_SETSIZE , &s.active_socket_read, &s.active_socket_write, NULL , &timeout)) == -1)//Select helps manipulate multiple active clients (cleaner way of handling it than using threads)//check if ready to read and write at same time //changes read and write sets, only keeps the active ones //returns the total
  {
      std::cout << "Error: select failed" << std::endl;
      exit(1);
  }
  else if (ret == 0) //If return is zero timeout happened
	{
  	disconnect_all(s, config);
		wait_connexion(s, config);
	}
  else if (FD_ISSET(s.server_socket, &s.active_socket_read)) //If returns true, something happened on server socket, meaning a new connexion occured
    new_incoming_connection(s, config);
}

void handle_write(t_server &s, t_config &config)
{
  (void)config;
  int message_ret; //Receive message lenght to add a /0 at end str

  for (unsigned int i = 0; i < SOMAXCONN; i++)
  {
	  if (FD_ISSET(s.client_socket[i] , &s.active_socket_write))
	  {
			if ((message_ret = send(s.client_socket[i], s.answer[s.client_socket[i]].c_str(), s.answer[s.client_socket[i]].size(), 0)) == -1)
        P("Error: send failed");
			if (message_ret < s.answer[s.client_socket[i]].size())
				s.answer[s.client_socket[i]] = s.answer[s.client_socket[i]].substr(message_ret + 1, s.answer[s.client_socket[i]].size());
			else
				s.answer.erase(s.client_socket[i]);
	  }
  }
}

void handle_read(t_server &s, t_config &conf)
{
	t_http_req *req = new t_http_req;
	std::string request;
	std::map<int, std::string>::iterator requests;

  get_client_request(s, conf);
	if (s.requests.size() != 0)
	{
		requests = s.requests.begin(); //socket is iterator map with socket fd (first) and request text (second)
		while(requests != s.requests.end()) //If receiving multiple socket connections at same time handle them all
		{
				change_directory("/frontend");
				parse_http_request(req, requests->second, conf);
				if (req->ready == true)
					get_answer(requests, *req, conf, s);
        else
				    requests++;
				change_directory("/..");
		}
	}
  delete req;
}

void server(t_server &s, t_config &conf)
{
	while(true)
  {
      wait_connexion(s, conf);
			handle_write(s, conf);
			handle_read(s, conf);
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

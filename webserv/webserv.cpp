#include "connexion/connexion.hpp"

void wait_connexion(t_config &c)
{
  int ret;
  struct timeval timeout = {1, 0};

  reset_sockets(c.s);
  c.timeout = false;
  if ((ret = select(FD_SETSIZE , &c.s.active_socket_read, &c.s.active_socket_write, NULL , &timeout)) == -1)//Select helps manipulate multiple active clients (cleaner way of handling it than using threads)//check if ready to read and write at same time //changes read and write sets, only keeps the active ones //returns the total
  {
      std::cout << "Error: select failed" << std::endl;
      wait_connexion(c);
  }
  else if (ret == 0) //If return is zero timeout happened
	{
  		disconnect_all(c.s);
      c.timeout = true;
	}
  else if (FD_ISSET(c.s.server_socket, &c.s.active_socket_read))
    new_incoming_connection(c.s);
}

void handle_write(t_server &s)
{
  int message_len; //Receive message lenght to add a /0 at end str

  for (unsigned int i = 0; i < s.fd_max; i++)
  {
	  if (FD_ISSET(s.client_socket[i] , &s.active_socket_write))
	  {
      P(s.answer[s.client_socket[i]]);
			if ((message_len = send(s.client_socket[i], s.answer[s.client_socket[i]].c_str(), s.answer[s.client_socket[i]].size(), 0)) == -1)
      {
        P("Error: send failed");
        P(strerror(errno)); //If send fails due to client disconnection, disconnect client
        client_restart(s, i);
      }
      // else if (message_len == 0) //If message lenght is equal to 0, the client socket closed connection, thus disconnect
      //   client_disconnection(s, i);
			else if ((size_t)message_len < s.answer[s.client_socket[i]].size())
				s.answer[s.client_socket[i]] = s.answer[s.client_socket[i]].substr(message_len + 1, s.answer[s.client_socket[i]].size());
			else
				s.answer.erase(s.client_socket[i]);
	  }
  }
}

void handle_read(t_config &c)
{
	std::map<int, t_http_req>::iterator requests;

  get_client_request(c.s);
	if (c.s.requests.size() != 0)
	{

		requests = c.s.requests.begin(); //socket is iterator map with socket fd (first) and request text (second)
		while(requests != c.s.requests.end()) //If receiving multiple socket connections at same time handle them all
		{
				parse_http_request(requests->second, requests->second.complete_request, c);
				if (requests->second.ready == true)
					get_answer(requests, requests->second, c);
        else
				    requests++;
		}
	}
}

void launch_server(std::list<t_config> &c)
{
  std::list<t_config>::iterator server = c.begin();

  try //catch exceptions if an exception occurs in catch block
  {
	   while(true)
     {
       try //catch exceptions during server working
       {
         if (server == c.end())
          server = c.begin();
         wait_connexion(*server);
         if ((*server).timeout == false)
         {
	        handle_write((*server).s);
	        handle_read(*server);
         }
         server++;
       }
       catch (const std::out_of_range &e) //If out of range error, parsing error, meaning parsing received wrong request thus restart client request
       {
         P(e.what());
         client_restart_all((*server).s);
       }
       catch (const std::exception &e) //All other exceptions catch them to keep server running, return internal server error to clients
       {
         P(e.what());
         internal_server_error((*server).s);
       }
     }
  }
  catch (std::exception &e)
  {
    P("Exception occured in catch block: relaunch server");
    launch_server(c);
  }
}

int main(int argc , char *argv[])
{
	std::list<t_config> config;

	if (argc != 2)
	{
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
		exit(1);
	}
	signal(SIGPIPE, SIG_IGN); //Ignore closed pipe error - Client closes connextion when trying to send
	parse_config(argv[1], config);
  for (std::list<t_config>::iterator i = config.begin(); i != config.end(); i++)
	 setup_server(*i);
  change_directory("/webserv/frontend");
	launch_server(config);
  return 0;
}

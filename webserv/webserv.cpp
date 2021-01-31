#include "connexion/connexion.hpp"

void wait_connexion(std::list<t_config> &c, t_active_socket &active_socket)
{
	int ret;
	struct timeval timeout = {1, 0};

	all_servers_reset_sockets(c, active_socket);
	if ((ret = select(FD_SETSIZE , &active_socket.read, &active_socket.write, NULL , &timeout)) == -1)//Select helps manipulate multiple active clients (cleaner way of handling it than using threads)//check if ready to read and write at same time //changes read and write sets, only keeps the active ones //returns the total
	{
		std::cout << "Error: select failed" << std::endl;
		wait_connexion(c, active_socket);
	}
	else if (ret == 0) //If return is zero timeout happened
	{
		// P("timeout");
		all_servers(c, active_socket, disconnect_all);
		wait_connexion(c, active_socket);
	}
	else
		all_servers(c, active_socket, new_incoming_connection);
	// P("END");
	// P(ret);
}

void handle_write(t_server &s, t_active_socket &active_socket)
{
	int message_len; //Receive message lenght to add a /0 at end str

	for (unsigned int i = 0; i < s.fd_max; i++)
	{
		if (FD_ISSET(s.client_socket[i] , &active_socket.write))
		{
			//   P(s.answer[s.client_socket[i]]);
			if ((message_len = send(s.client_socket[i], s.answer[s.client_socket[i]].c_str(), s.answer[s.client_socket[i]].size(), 0)) == -1)
			{
				P("Error: send failed");
				P(strerror(errno)); //If send fails due to client disconnection, disconnect client
				client_restart(s, i);
			}
			// else if (message_len == 0) //If message lenght is equal to 0, the client socket closed connection, thus disconnect
			//   client_disconnection(s, i);
			else if ((size_t)message_len < s.answer[s.client_socket[i]].size())
				s.answer[s.client_socket[i]] = s.answer[s.client_socket[i]].substr(message_len, s.answer[s.client_socket[i]].size());
			else
				s.answer.erase(s.client_socket[i]);
		}
	}
}

void handle_read(t_config &c, t_active_socket &active_socket)
{
	std::map<int, t_http_req>::iterator requests;

	get_client_request(c.s, active_socket);
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
	t_active_socket active_socket;
	std::list<t_config>::iterator server = c.begin();

	try //catch exceptions if an exception occurs in catch block
	{
		while(true)
		{
			try //catch exceptions during server working
			{
				wait_connexion(c, active_socket);
				while (server != c.end())
				{
					handle_read(*server, active_socket);
					handle_write((*server).s, active_socket);
					server++;
				}
				server = c.begin();
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
		all_servers(c, active_socket, disconnect_all);
		launch_server(c);
	}
}

// void check_config(std::list<t_config> &c)
// {
//   for (std::list<t_config>::iterator first = c.begin(); first != c.end(); first++)
//     for (std::list<t_config>::iterator second = c.begin(); second != c.end(); second++)
//       if (first != second && (*first).host == (*second).host && (*first).port == (*second).port)
//       {
//         P("Error: different server blocks point on same host and port");
//         exit(1);
//       }
// }

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
	// check_config(config);
	for (std::list<t_config>::iterator i = config.begin(); i != config.end(); i++)
		setup_server(*i);
	change_directory("/webserv/frontend");
	launch_server(config);
	return 0;
}

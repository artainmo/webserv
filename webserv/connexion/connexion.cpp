#include "connexion.hpp"

void setup_server(t_config &c)
{
    c.s.addrlen = sizeof(c.s.address);

    //initialise client sockets to null or inactive
    for (unsigned int i = 0; i < SOMAXCONN; i++)
    {
        c.s.client_socket[i] = 0;
    }

    //Creating server socket file descriptor
    //AF_INET = IPv4 | AF_INET6 = IPv6
    //SOCK_STREAM = TCP | STOCK_DGRAM = UDP
    //0 is protocol value for IP
    if ((c.s.server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "Error: socket failed" << std::endl;
        exit(1);
    }
	   c.s.fd_max = c.s.server_socket;

    //set server socket to allow multiple connections
    int opt = 1;
    if(setsockopt(c.s.server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
    {
      std::cout << "Error: sotsockopt failed" << std::endl;
      exit(1);
    }
    //type of socket created
    c.s.address.sin_family = AF_INET; //= IPv4
    c.s.address.sin_addr.s_addr = inet_addr(c.host.c_str()); //INADDR_ANY Makes the socket bound to all network interfaces on the host, important when server offers services to multiple networks //server address can only bind to network interfaces  //inet_addr function is used to transform string to IPv4 decimal notation
    try
    {
      c.s.address.sin_port = htons(std::stoi(c.port.front())); //decode port adress from host byte order to network byte order
    }
    catch(std::exception &e)
    {
      P("Error: port is not a number in .conf");
      exit(1);
    }

    if (bind(c.s.server_socket, (struct sockaddr *)&c.s.address, c.s.addrlen) == -1)
    {
        std::cout << "Error: bind failed" << std::endl;
        P(strerror(errno));
        exit(1);
    }
    std::cout << "Host: " << c.host << " listening on port: " << c.port.front() << std::endl;
    //SOMAXCONN is constant of max number of client requests we could wait for
    if (listen(c.s.server_socket, SOMAXCONN) == -1)
    {
        std::cout << "Error: listen failed" << std::endl;
        exit(1);
    }
}

const char *internal_server_error_exc::what() const _NOEXCEPT
{
  return "Internal server error";
}

void internal_server_error(t_server &s)
{
  t_config conf; //Empty conf just for error page parameter will have no effect, 500 error will be returned independently of default_error_page

	for (unsigned int i = 0 ; i < s.fd_max ; i++)
	{
		if(s.client_socket[i] > 0)
    {
      s.answer[s.client_socket[i]] = error_page(500, "None", conf); //method does not matter
      s.requests[s.client_socket[i]].ready = true;
    }
	}
}

void client_restart(t_server &s, unsigned int i) //Connection has been closed but restarted, do not disconnect but restart answer and request
{
  //Close the socket and reset to zero for re-use
  s.answer.erase(s.client_socket[i]); //does not need to be protected erase key_type
  s.requests.erase(s.client_socket[i]);
}

void client_restart_all(t_server &s)
{
	for (unsigned int i = 0 ; i < s.fd_max ; i++)
	{
		if(s.client_socket[i] > 0)
			client_disconnection(s, i);
	}
}

void client_disconnection(t_server &s, unsigned int i)
{
  //Show to debug
  getpeername(s.client_socket[i] , (struct sockaddr*)&s.address , (socklen_t*)&s.addrlen);
  printf("Host disconnected\n-ip: %s\n-port: %d \n" , inet_ntoa(s.address.sin_addr) , ntohs(s.address.sin_port));

  //Close the socket and reset to zero for re-use
  s.answer.erase(s.client_socket[i]); //does not need to be protected erase key_type
  s.requests.erase(s.client_socket[i]);
  close(s.client_socket[i]);
  s.client_socket[i] = 0;
}

void disconnect_all(t_server &s, t_active_socket &active_socket)
{
  FD_ZERO(&active_socket.read); //Deactivate the active sockets
  FD_ZERO(&active_socket.write); //Deactivate the active sockets

	for (unsigned int i = 0 ; i < s.fd_max ; i++)
	{
		if(s.client_socket[i] > 0)
			client_disconnection(s, i);
	}
}

void add_new_socket_to_active_sockets(t_server &s)
{
  for (unsigned int i = 0; i < s.fd_max; i++)
  {
      if(s.client_socket[i] == 0)
      {
          s.client_socket[i] = s.connected_socket;
          break;
      }
  }
}

void get_client_request(t_server &s, t_active_socket &active_socket)
{
  int message_len = -1; //Receive message lenght to add a /0 at end str
  char message_buffer[1000001];  //Received message is taken into a char* message_buffer because we use C functions
  std::string message;

  for (unsigned int i = 0; i < s.fd_max; i++)
  {
      if (FD_ISSET(s.client_socket[i] , &active_socket.read)) //If client socket still in active sockets, a request exists from that client
      {
          if ((message_len = recv(s.client_socket[i] , message_buffer, 1000000, 0)) == -1) //Read the incoming message //MSG_PEEK //read whole message
          {
            P("Error: recv failed");
            P(strerror(errno)); //If recv fails due to client disconnection, disconnect client
            client_restart(s, i);
          }
          else if (message_len == 0) //If incoming message lenght is equal to 0, the client socket closed connection
            client_disconnection(s, i);
          else
          {
              message_buffer[message_len] = '\0'; //End message buffer with terminating /0
              message = message_buffer;
              s.requests[s.client_socket[i]].complete_request += message; //Create key in map with its value
              P("----------------------------------------------");
			        P("Message:\n" << message);
              P("----------------------------------------------");
          }
      }
  }
}

void reset_sockets(t_server &s, t_active_socket &active_socket)
{
  FD_SET(s.server_socket, &active_socket.read); //Add the server socket to active sockets
  FD_SET(s.server_socket, &active_socket.write); //Add the server socket to active sockets

  //Add active client sockets to the active sockets
  for (unsigned int i = 0 ; i < s.fd_max; i++)
  {
      if(s.client_socket[i] > 0)
      {
          FD_SET(s.client_socket[i] , &active_socket.read);
		      if (s.answer.find(s.client_socket[i]) != s.answer.end()) //Do not add the sockets that are being answered
        	 FD_SET(s.client_socket[i] , &active_socket.write);
		   }
  }
}

void all_servers_reset_sockets(std::list<t_config> &c, t_active_socket &active_socket)
{
  FD_ZERO(&active_socket.read); //Deactivate the active sockets
  FD_ZERO(&active_socket.write); //Deactivate the active sockets

  for (std::list<t_config>::iterator i = c.begin(); i != c.end(); i++)
			reset_sockets((*i).s, active_socket);
}

void set_new_incoming_connection(t_server &s)
{
    //Extracts first connection request in queue and returns new connected socket (server - client)
    if ((s.connected_socket = accept(s.server_socket, (struct sockaddr *)&s.address, (socklen_t*)&s.addrlen)) == -1)
    {
      std::cout << "Error: listen failed" << std::endl;
      throw internal_server_error_exc();
    }
	if ((unsigned int)s.connected_socket > s.fd_max)
		s.fd_max = s.connected_socket;
  fcntl(s.connected_socket, F_SETFL, O_NONBLOCK);
  add_new_socket_to_active_sockets(s);
  printf("New connection\n-socket fd: %d\n-ip: %s\n-port: %d\n" , s.connected_socket , inet_ntoa(s.address.sin_addr) , ntohs(s.address.sin_port)); //Show to debug
}

void new_incoming_connection(t_server &s, t_active_socket &active_socket)
{
  if (FD_ISSET(s.server_socket, &active_socket.read)) // tests to see if a file descriptor is part of the set
    set_new_incoming_connection(s);
}

void all_servers(std::list<t_config> &c, t_active_socket &active_socket, void (*f)(t_server &, t_active_socket &))
{
  for (std::list<t_config>::iterator i = c.begin(); i != c.end(); i++)
			f((*i).s, active_socket);
}

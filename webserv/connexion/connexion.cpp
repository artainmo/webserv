#include "connexion.hpp"

void setup_server(t_server &s, t_config &config)
{
    (void)config;
    s.addrlen = sizeof(s.address);

    //initialise client sockets to null or inactive
    for (unsigned int i = 0; i < SOMAXCONN; i++)
    {
        s.client_socket[i] = 0;
    }

    //Creating server socket file descriptor
    //AF_INET = IPv4 | AF_INET6 = IPv6
    //SOCK_STREAM = TCP | STOCK_DGRAM = UDP
    //0 is protocol value for IP
    if ((s.server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "Error: socket failed" << std::endl;
        exit(1);
    }
	   s.fd_max = s.server_socket;

    //set server socket to allow multiple connections
    int opt = 1;
    if(setsockopt(s.server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
    {
      std::cout << "Error: sotsockopt failed" << std::endl;
      exit(1);
    }
    //type of socket created
    s.address.sin_family = AF_INET; //= IPv4
    s.address.sin_addr.s_addr = INADDR_ANY; //Makes the socket bound to all network interfaces on the host, important when server offers services to multiple networks
    s.address.sin_port = htons(config.port); //decode port adress with htons

    if (bind(s.server_socket, (struct sockaddr *)&s.address, s.addrlen) == -1)
    {
        std::cout << "Error: bind failed" << std::endl;
        P(strerror(errno));
        exit(1);
    }

    std::cout << "Listening on port: ";
    std:: cout << config.port << std::endl;
    //Server socket waits for a client socket to connect
    //SOMAXCONN is constant of max number of client requests we could wait for
    if (listen(s.server_socket, SOMAXCONN) == -1)
    {
        std::cout << "Error: listen failed" << std::endl;
        exit(1);
    }
}

void client_restart(t_server &s, unsigned int i) //Connection has been closed but restarted, do not disconnect but restart answer and request
{
  //Close the socket and reset to zero for re-use
  s.answer.erase(s.client_socket[i]); //does not need to be protected erase key_type
  s.requests.erase(s.client_socket[i]);
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

void disconnect_all(t_server &s, t_config &config)
{
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

void new_incoming_connection(t_server &s)
{
    //Extracts first connection request in queue and returns new connected socket (server - client)
    if ((s.connected_socket = accept(s.server_socket, (struct sockaddr *)&s.address, (socklen_t*)&s.addrlen)) == -1)
    {
      std::cout << "Error: listen failed" << std::endl;
      exit(1);
    }
	if (s.connected_socket > s.fd_max)
		s.fd_max = s.connected_socket;
    fcntl(s.connected_socket, F_SETFL, O_NONBLOCK);
    add_new_socket_to_active_sockets(s);
    printf("New connection\n-socket fd: %d\n-ip: %s\n-port: %d\n" , s.connected_socket , inet_ntoa(s.address.sin_addr) , ntohs(s.address.sin_port)); //Show to debug
}

void get_client_request(t_server &s)
{
  int message_len = -1; //Receive message lenght to add a /0 at end str
  char message_buffer[1000001];  //Received message is taken into a char* message_buffer because we use C functions
  std::string message;

  for (unsigned int i = 0; i < s.fd_max; i++)
  {
      if (FD_ISSET(s.client_socket[i] , &s.active_socket_read)) //If client socket still in active sockets, a request exists from that client
      {
          if ((message_len = recv(s.client_socket[i] , message_buffer, 1000000, 0)) == -1) //Read the incoming message //MSG_PEEK //read whole message
          {
            P("Error: recv failed");
            P(strerror(errno)); //If recv fails due to client disconnection, disconnect client
            // if (errno == 54) //Functions properly!!
            client_restart(s, i);
          }
          else if (message_len == 0) //If incoming message lenght is equal to 0, the client socket closed connection
            client_disconnection(s, i);
          else
          {
              message_buffer[message_len] = '\0'; //End message buffer with terminating /0
              message = message_buffer;
              s.requests[s.client_socket[i]].complete_request += message; //Create key in map with its value
              // if (s.requests[s.client_socket[i]].size() > 100000000)
              //   s.requests[s.client_socket[i]] += "0"; //0 to tell parser end of chunked message
            //   P("Size message char:" << message_len);
			//   P("Message: " << message);
              // P("Size message std::string:" << s.requests[s.client_socket[i]].size());
          }
      }
  }
}

void reset_sockets(t_server &s)
{
  FD_ZERO(&s.active_socket_read); //Deactivate the active sockets
  FD_SET(s.server_socket, &s.active_socket_read); //Add the server socket to active sockets
  FD_ZERO(&s.active_socket_write); //Deactivate the active sockets
  FD_SET(s.server_socket, &s.active_socket_write); //Add the server socket to active sockets

  //Add active client sockets to the active sockets
  //std::cout << "SOCKET REPLY: " << s.requests.size() << std::endl;
  for (unsigned int i = 0 ; i < s.fd_max; i++)
  {
      if(s.client_socket[i] > 0)
      {
          FD_SET(s.client_socket[i] , &s.active_socket_read);
		      if (s.answer.find(s.client_socket[i]) != s.answer.end()) //Do not add the sockets that are being answered
        	 FD_SET(s.client_socket[i] , &s.active_socket_write);
		   }
  }
}

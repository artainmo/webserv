//create_socket
//connect
//receive
std::string *hello = "Hello from server";
int port = 80

void create_socket()
{
    int server_fd;
    int connected_socket;
    int request_lenght;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024];

    // Creating socket file descriptor
    //AF_INET = IPv4 | AF_INET6 = IPv6
    //SOCK_STREAM = TCP | STOCK_DGRAM = UDP
    //0 is protocol value for IP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "Error: socket failed" << std::endl;
        exit(1);
    }

    //Setting sockaddr struct
    address.sin_family = AF_INET; //= IPv4
    address.sin_addr.s_addr = INADDR_ANY; //Makes the socket bound to all network interfaces on the host, important when server offers services to multiple networks
    address.sin_port = htons(port); //decode port adress with htons

    //Bind to a specific ip address and port number
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cout << "Error: bind failed" << std::endl;
        exit(1);
    }

}

void connect()
{
  // Creating socket file descriptor
  //AF_INET = IPv4 | AF_INET6 = IPv6
  //SOCK_STREAM = TCP | STOCK_DGRAM = UDP
  //0 is protocol value for IP
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      std::cout << "Error: socket failed" << std::endl;
      exit(1);
  }

  //Setting sockaddr struct
  address.sin_family = AF_INET; //= IPv4
  address.sin_addr.s_addr = INADDR_ANY; //Makes the socket bound to all network interfaces on the host, important when server offers services to multiple networks
  address.sin_port = htons(port); //decode port adress with htons

  //Bind to a specific ip address and port number
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
  {
      std::cout << "Error: bind failed" << std::endl;
      exit(1);
  }
}

void receive()
{
  //Server socket waits for a client socket to connect
  //SOMAXCONN is constant of max number of client requests we could wait for
  if (listen(server_fd, SOMAXCONN) == -1)
  {
      std::cout << "Error: listen failed" << std::endl;
      exit(1);
  }

  //Extracts first connection request in queue and returns new connected socket
  if ((connected_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1)
  {
      std::cout << "Error: accept failed" << std::endl;
      exit(1);
  }

  //Read the message and put it into beffer
  request_lenght = read(connected_socket , buffer, strlen(buffer), 0);
}

void answer()
{
  printf("%s\n",buffer );
  send(new_socket , hello , strlen(hello) , 0 );
  printf("Hello message sent\n");
  return 0;
}

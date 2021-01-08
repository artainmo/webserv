#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h> 
#include <arpa/inet.h>

#include "main.hpp"

#define PORT 80
#define BUFSIZE 4096

typedef struct sockaddr_in SA_IN ;

std::string http_request_example = "GET / HTTP/1.1\r\n"
			"Accept-Charsets: dwdwdq dwqdwqdwqdwq\r\n"
            "Accept-Language: en-us\r\n"
			"Allow: xwdwdw dwqdwqdwq\r\n"
			"Authorization: dwdwqd dwqdwqdwq\r\n"
			"Content-Language: dfeefe efewf\r\n"
			"Content-Length: dwdwdw wdwdqw\r\n"
			"Content-Location: sdwdwqdwdwd\r\n"
			"Content-Type: ddw dwqdwqd\r\n"
			"Date: 09-9w2-1w1\r\n"
            "Host: localhost:8080\r\n"
			"Last-Modified: 424443 3424324\r\n"
			"Location: c;d';e\r\n"
			"Referer: dwwdwqd dwqdqwdwq\r\n"
			"Retry-After: dwdqwdw wdwqdqwdw\r\n"
			"Server: dwdwdwdwdwdwd\r\n"
			"Transfer-Encoding: fdsfsf34343\r\n"
            "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/604.5.6 (KHTML, like Gecko) Version/11.0.3 Safari/604.5.6\r\n"
			"WWW_Authenticate: 2323wwdwdw\r\n"
            "\r\n"
            "Usually GET requests don\'t have a body\r\n"
            "But I don\'t care in this case :)";


void check(int val, std::string str)
{
	if (val < 0)
		std::cout << str << std::endl;
}

int	setup_server(short port, int backlog)
{
	int server_socket;
	SA_IN server_addr;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	check(server_socket, "Failed to create a socket!");

	// initialize the address struct
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// bind the address to an IP / Port
	check(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)),
	"Failed to bind to an IP/PORT");

	// Mark the socket for listening in
	check(listen(server_socket, backlog), "Failed to listen !");
	return server_socket;
}

int	accept_new_connection(int & server_socket)
{
	size_t addr_size = sizeof(SA_IN);
	int client_socket;

	SA_IN client_addr;

	client_socket = accept(server_socket,
							(sockaddr*)&client_addr,
							(socklen_t*) &addr_size);
		check(client_socket, "Accept Failed !");
	return client_socket;
}

void	*handle_connection(int & client_socket)
{
	char		buffer[BUFSIZE];
	size_t		bytes_read;
	int		msgsize = 0;
	char		actualpath[PATH_MAX + 1];
	t_http_req	*req;

	// clear the buffer
	bzero(buffer, 4096);

	//read the client's message -- the name of the file to read
	bytes_read = recv(client_socket, buffer, 4096, 0);
	check(bytes_read, "recv error");

// artainmo
	req = parse_http_request(buffer);

// Check si la connection doit rester ouverte ou pas

//std::cout << "REQUEST: " << buffer << std::endl;
//read file and send its contents to client	
//FILE *fp = fopen(actualpath, "r");

	FILE *html_data = fopen("index.html", "r");
	if (html_data == NULL)
	{
		std::cout << "ERROR(open): " << buffer << std::endl;
		return nullptr;
	}

	//read file contents and send them to client
	// This is a fine example program but rather insecure
	// a real program would probably limit the client to certain files

	fgets(buffer, 4096, html_data);

	char	http_header[4096] = "HTTP/1.1 200 OK\r\n\n";
	strcat(http_header, buffer);
	send(client_socket, http_header, sizeof(http_header), 0);
	close(client_socket);
	fclose(html_data);
	std::cout << "Closing connection" << std::endl;
	return nullptr;
}

int main(int argc, char **argv)
{
	t_config *config;
	t_http_req *req;

	if (argc != 2)
	{
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
		exit(1);
	}
	config = parse_config(argv[1]);

	int server_socket = setup_server(PORT, SOMAXCONN);

	fd_set current_sockets, ready_sockets;

	//initialize my current set
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);

	while (true)
	{
		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::cout << "select error" << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == server_socket)
				{
					// this is a new connection
					int client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets); // add the socket to the pool
				}
				else
				{
					// do whatever we do with connection
					handle_connection(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	return (0);
}

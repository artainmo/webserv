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
	t_server s; // = new t_server;
	t_config *config;
	t_http_req req;
	std::string request;

	signal(SIGPIPE, SIG_IGN);
	//bzero(&s, sizeof(s));
	if (argc != 2)
	{
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
		exit(1);
	}
	config = parse_config(argv[1]);
	setup_server(s, *config);
	while(true)
	{
		wait_connexion(s, *config);
		if (FD_ISSET(s.server_socket, &s.active_socket_read)) //If returns true, something happened on server socket, meaning a new connexion occured
			new_incoming_connection(s, *config);
		change_directory(std::string("/frontend"));
		if ((request = get_client_request(s, *config)) != std::string("None") && request.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
		{
			/*P("--------------------------------------------------------------------------");
			P("YVES REAL REQUEST:");
			std::cout << ">|" << request << "|<" << "size:" << request.size() << std::endl;
			//	P(req); //test
			P("--------------------------------------------------------------------------");
			*/req = parse_http_request(request, *config);
			answer_http_request(s, req, *config);
		}
		change_directory("/..");
	}
	return 0;
}

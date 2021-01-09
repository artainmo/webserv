#include "main.hpp"

int main(int argc, char **argv)
{
	t_config *config;
	t_http_req *req;
	t_connexion *conn;

	if (argc != 2)
	{
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
		exit(1);
	}
	config = parse_config(argv[1]);
	while (true)
	{
		http_request = receive_http_request(config, conn);
		req = parse_http_request(http_request_example);
		answer_http_request(req, conn);
	}
}

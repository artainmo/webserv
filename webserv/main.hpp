#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <list>

typedef struct s_config_routes
{
	list http_methods;
	std::string start_search_dir;
	boolean directory_listing;
	std::string default_file_if_request_directory;

} t_routes

typedef struct s_config
{
	std::string host;
	std::string port;
	std::string server_name;
	std::string default_error_page;
	std::string client_limit_body_size;
	t_routes routes;

} t_config

t_config config_file(std::string path);

#endif

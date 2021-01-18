#include "parsing.hpp"

void init_cgi(t_location &loc)
{
	loc.CGI = new t_CGI;
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->AUTH_TYPE = std::string("None");
	loc.CGI->CONTENT_LENGTH = std::string("None");
	loc.CGI->CONTENT_TYPE = std::string("None");
	loc.CGI->GATEWAY_INTERFACE = std::string("None");
	loc.CGI->PATH_INFO = std::string("None");
	loc.CGI->PATH_TRANSLATED = std::string("None");
	loc.CGI->QUERY_STRING = std::string("None");
	loc.CGI->REMOTE_ADDR = std::string("None");
	loc.CGI->REMOTE_INDENT = std::string("None");
	loc.CGI->REMOTE_USER = std::string("None");
	loc.CGI->REQUEST_METHOD = std::string("None");
	loc.CGI->REQUEST_URI = std::string("None");
	loc.CGI->SCRIPT_NAME = std::string("None");
	loc.CGI->SERVER_NAME = std::string("None");
	loc.CGI->SERVER_PORT = std::string("None");
	loc.CGI->SERVER_PROTOCOL = std::string("None");
	loc.CGI->SERVER_SOFTWARE = std::string("None");
}

void parse_cgi(std::string &line, t_location &loc)
{
	if (loc.CGI == 0)
		init_cgi(loc);
	line = following_content(line, "fastcgi_param");
	if (check_line(line, "AUTH_TYPE"))
		loc.CGI->AUTH_TYPE = following_content(line, "AUTH_TYPE");
	if (check_line(line, "CONTENT_LENGTH"))
		loc.CGI->CONTENT_LENGTH = following_content(line, "CONTENT_LENGTH");
	if (check_line(line, "CONTENT_TYPE"))
		loc.CGI->CONTENT_TYPE = following_content(line, "CONTENT_TYPE");
	if (check_line(line, "GATEWAY_INTERFACE"))
		loc.CGI->GATEWAY_INTERFACE = following_content(line, "GATEWAY_INTERFACE");
	if (check_line(line, "PATH_INFO"))
		loc.CGI->PATH_INFO = following_content(line, "PATH_INFO");
	if (check_line(line, "PATH_TRANSLATED"))
		loc.CGI->PATH_TRANSLATED = following_content(line, "PATH_TRANSLATED");
	if (check_line(line, "QUERY_STRING"))
		loc.CGI->QUERY_STRING = following_content(line, "QUERY_STRING");
	if (check_line(line, "REMOTE_ADDR"))
		loc.CGI->REMOTE_ADDR = following_content(line, "REMOTE_ADDR");
	if (check_line(line, "REMOTE_INDENT"))
		loc.CGI->REMOTE_INDENT = following_content(line, "REMOTE_INDENT");
	if (check_line(line, "REMOTE_USER"))
		loc.CGI->REMOTE_USER = following_content(line, "REMOTE_USER");
	if (check_line(line, "REQUEST_METHOD"))
		loc.CGI->REQUEST_METHOD = following_content(line, "REQUEST_METHOD");
	if (check_line(line, "REQUEST_URI"))
		loc.CGI->REQUEST_URI = following_content(line, "REQUEST_URI");
	if (check_line(line, "SCRIPT_NAME"))
		loc.CGI->SCRIPT_NAME = following_content(line, "SCRIPT_NAME");
	if (check_line(line, "SERVER_NAME"))
		loc.CGI->SERVER_NAME = following_content(line, "SERVER_NAME");
	if (check_line(line, "SERVER_PORT"))
		loc.CGI->SERVER_PORT = following_content(line, "SERVER_PORT");
	if (check_line(line, "SERVER_PROTOCOL"))
		loc.CGI->SERVER_PROTOCOL = following_content(line, "SERVER_PROTOCOL");
	if (check_line(line, "SERVER_SOFTWARE"))
		loc.CGI->SERVER_PROTOCOL = following_content(line, "SERVER_SOFTWARE");
}

void parse_location_line_file_extensions(std::string &line, t_location &loc)
{
	std::string parsed;
	std::string extension;

	parsed = parse_between(line, '(', ')');
	if (parsed != std::string("None"))
	{
		loc.file_extensions.pop_front();
		while(parsed != std::string("None"))
		{
			extension = parse_until(parsed, "|");
			loc.file_extensions.push_back(extension);
			parsed = following_content(parsed, extension);
		}
	}
}

void parse_location_line_link_extension(std::string &line, t_location &loc)
{
	std::string parsed;

	parsed = parse_between(line, '/', '/', false);
	if (parsed != std::string("None"))
		loc.link_extension = parsed;
}

void init_location(t_location &loc)
{
	loc.file_extensions.push_back("ALL");
	loc.link_extension = "None";
	loc.http_methods.push_back("None");
	loc.root = "/";
	loc.directory_listing = std::string("false");
	loc.default_file_if_request_directory = "None";
	loc.CGI = 0;
	loc.file_upload_location = "None";
}

void parse_location(t_config &conf, std::ifstream &fd, std::string &line) //In location block
{
	t_location *loc = new t_location;

	init_location(*loc);
	parse_location_line_link_extension(line, *loc);
	parse_location_line_file_extensions(line, *loc);
	while (getlinecut(fd, line) && !check_line(line, "}"))
	{
		if (check_line(line, "http_methods"))
			loc->http_methods = following_contents(line, "http_methods");
		else if (check_line(line, "root"))
			loc->root = following_content(line, "root");
		else if (check_line(line, "directory_listing"))
			loc->directory_listing = following_content(line, "directory_listing");
		else if (check_line(line, "default_file_if_request_directory"))
			loc->default_file_if_request_directory = following_content(line, "default_file_if_request_directory");
		else if (check_line(line, "fastcgi_param"))
			parse_cgi(line, *loc);
		else if (check_line(line, "file_upload_location"))
			loc->file_upload_location = following_content(line, "file_upload_location");
	}
	conf.locations.push_back(*loc);
	delete loc;
}


void  parse(t_config &conf, std::ifstream &fd)
{
	std::string line;

	while (std::getline(fd, line) && !check_line(line, "server"))
		pass;
	//In server block
	while (getlinecut(fd, line) && !check_line(line, "}"))
	{
		if (check_line(line, "host"))
			conf.host = following_content(line, "host");
		else if (check_line(line, "port"))
		{
			try
			{
				conf.port = std::stoi(following_content(line, "port"));
			}
			catch(std::exception &e)
			{
				P(e.what());
			}
		}
		else if (check_line(line, "server_name"))
			conf.server_name = following_content(line, "server_name");
		else if (check_line(line, "default_error_page"))
			conf.default_error_page = following_content(line, "default_error_page");
		else if (check_line(line, "body_size_limit"))
			conf.body_size_limit = following_content(line, "body_size_limit");
		else if (check_line(line, "location"))
			parse_location(conf, fd, line);
	}
	(void)conf;
}

void default_init(t_config &conf)
{
	conf.host = "None";
	conf.port = -1;
	conf.server_name = "None";
	conf.default_error_page = "None";
	conf.body_size_limit = "None";
}

t_config *parse_config(std::string path)
{
	t_config *ret = new t_config;
	std::ifstream fd(path);

	if (!fd.is_open())
	{
		if (errno == 2)
			P("Error: no such file or directory");
		else if (errno == 13)
			P("Error: permission denied");
		exit(1);
	}
	default_init(*ret);
	parse(*ret, fd);
	fd.close();
	if(ret->port == -1)
	{
		P("Error: port not specified");
		exit(1);
	}
	//show_conf(*config);
	return (ret);
}

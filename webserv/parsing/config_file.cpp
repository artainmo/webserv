#include "../main.hpp"

void parse_http_methods(std::string &line, t_location &loc)
{
	std::string methods;
	std::string method;

	loc.http_methods.pop_front();
	methods = following_content(line, "http_methods");
	while(methods != std::string("None"))
	{
		method = parse_until(methods, ' ');
		loc.http_methods.push_back(method);
		methods = following_content(methods, method);
	}
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
			extension = parse_until(parsed, '|');
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
			parse_http_methods(line, *loc);
		if (check_line(line, "root"))
			loc->root = following_content(line, "root");
		if (check_line(line, "directory_listing"))
			loc->directory_listing = following_content(line, "directory_listing");
		if (check_line(line, "default_file_if_request_directory"))
			loc->default_file_if_request_directory = following_content(line, "default_file_if_request_directory");
		if (check_line(line, "fastcgi"))
			pass;
		if (check_line(line, "file_upload_location"))
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
			conf.port = following_content(line, "port");
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
	conf.port = "None";
	conf.server_name = "None";
	conf.default_error_page = "None";
	conf.body_size_limit = "None";
}

t_config *parse_config(std::string path)
{
	t_config *ret = new t_config;
	std::ifstream fd;

	fd.open(path, std::fstream::in);
	default_init(*ret);
	parse(*ret, fd);
	show_conf(*ret);
	return (ret);	
}

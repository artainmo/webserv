#include "parsing.hpp"

void init_cgi(t_location &loc, t_config &conf)
{
	loc.CGI.active = true;
	loc.CGI.AUTH_TYPE = std::string("None");
	loc.CGI.CONTENT_LENGTH = std::string("0"); //If no body default is 0
	loc.CGI.CONTENT_TYPE = std::string("None");
	loc.CGI.GATEWAY_INTERFACE = std::string("None");
	loc.CGI.PATH_INFO = std::string("None");
	loc.CGI.PATH_TRANSLATED = std::string("None");
	loc.CGI.QUERY_STRING = std::string(""); //Empty by default
	loc.CGI.REMOTE_ADDR = std::string("None");
	loc.CGI.REMOTE_INDENT = std::string("None");
	loc.CGI.REMOTE_USER = std::string("None");
	loc.CGI.REQUEST_METHOD = std::string("None");
	loc.CGI.REQUEST_URI = std::string("None");
	loc.CGI.SCRIPT_NAME = std::string("0");
	loc.CGI.SERVER_NAME = conf.server_name;
	loc.CGI.SERVER_PORT = conf.port.front();
	loc.CGI.SERVER_PROTOCOL = std::string("HTTP/1.1");
	loc.CGI.SERVER_SOFTWARE = std::string("None");
	loc.CGI.SECRET = std::string("None");
}

void parse_cgi(std::string &line, t_location &loc, t_config &conf)
{
	if (!loc.CGI.active)
		init_cgi(loc, conf);
	line = following_content(line, "fastcgi_param");
	if (check_line(line, "AUTH_TYPE"))
		loc.CGI.AUTH_TYPE = following_content(line, "AUTH_TYPE");
	if (check_line(line, "CONTENT_LENGTH"))
		loc.CGI.CONTENT_LENGTH = following_content(line, "CONTENT_LENGTH");
	if (check_line(line, "CONTENT_TYPE"))
		loc.CGI.CONTENT_TYPE = following_content(line, "CONTENT_TYPE");
	if (check_line(line, "GATEWAY_INTERFACE"))
		loc.CGI.GATEWAY_INTERFACE = following_content(line, "GATEWAY_INTERFACE");
	if (check_line(line, "PATH_INFO"))
		loc.CGI.PATH_INFO = following_content(line, "PATH_INFO");
	if (check_line(line, "PATH_TRANSLATED"))
		loc.CGI.PATH_TRANSLATED = following_content(line, "PATH_TRANSLATED");
	if (check_line(line, "QUERY_STRING"))
		loc.CGI.QUERY_STRING = following_content(line, "QUERY_STRING");
	if (check_line(line, "REMOTE_ADDR"))
		loc.CGI.REMOTE_ADDR = following_content(line, "REMOTE_ADDR");
	if (check_line(line, "REMOTE_INDENT"))
		loc.CGI.REMOTE_INDENT = following_content(line, "REMOTE_INDENT");
	if (check_line(line, "REMOTE_USER"))
		loc.CGI.REMOTE_USER = following_content(line, "REMOTE_USER");
	if (check_line(line, "REQUEST_METHOD"))
		loc.CGI.REQUEST_METHOD = following_content(line, "REQUEST_METHOD");
	if (check_line(line, "REQUEST_URI"))
		loc.CGI.REQUEST_URI = following_content(line, "REQUEST_URI");
	if (check_line(line, "SCRIPT_NAME"))
		loc.CGI.SCRIPT_NAME = following_content(line, "SCRIPT_NAME");
	if (check_line(line, "SERVER_NAME"))
		loc.CGI.SERVER_NAME = following_content(line, "SERVER_NAME");
	if (check_line(line, "SERVER_PORT"))
		loc.CGI.SERVER_PORT = following_content(line, "SERVER_PORT");
	if (check_line(line, "SERVER_PROTOCOL"))
		loc.CGI.SERVER_PROTOCOL = following_content(line, "SERVER_PROTOCOL");
	if (check_line(line, "SERVER_SOFTWARE"))
		loc.CGI.SERVER_PROTOCOL = following_content(line, "SERVER_SOFTWARE");
}

void parse_location_line_file_extensions(std::string &line, t_location &loc)
{
	std::string parsed;
	std::string extension;

	if ((parsed = parse_between(line, '(', ')')) != std::string("None")) //If () are used multiple file extensions will probably occur
	{
		loc.file_extensions.pop_front();
		while(parsed != std::string("None"))
		{
			extension = parse_until(parsed, "|");
			loc.file_extensions.push_back(extension);
			parsed = following_content(parsed, extension);
		}
		return ;
	}
	if (line.find('.') != std::string::npos && line.find('$') != std::string::npos)
	{
		loc.file_extensions.pop_front();
		loc.file_extensions.push_back(line.substr(line.find_last_of('.') + 1, line.find_last_of('$') - line.find_last_of('.') - 1));
	}
}

void parse_location_line_directory(std::string &line, t_location &loc)
{
	size_t start;
	size_t end;

	if ((start = line.find_first_of("/")) == std::string::npos)
		return ;
	end = line.find_last_of("/");
	if (start == end)
		loc.directory = "";
	else
		loc.directory = line.substr(start + 1, end - start - 1);
}

void init_location(t_location &loc, t_config &conf)
{
	loc.file_extensions.push_back("None");
	loc.directory = "None";
	loc.http_methods.push_back("ALL");
	loc.max_body = conf.body_size_limit;
	loc.root = conf.root;
	loc.index = conf.index;
	loc.directory_listing = std::string("false");
	loc.default_file_if_request_directory = "None";
	loc.CGI.active = false;
	loc.file_upload_location = "default/upload/default";
}

void parse_location(t_config &conf, std::ifstream &fd, std::string &line) //In location block
{
	t_location loc;

	init_location(loc, conf);
	parse_location_line_directory(line, loc);
	parse_location_line_file_extensions(line, loc);
	while (getlinecut(fd, line) && !check_line(line, "}"))
	{
		if (check_line(line, "http_methods"))
			loc.http_methods = following_contents(line, "http_methods");
		else if (check_line(line, "body_size_limit"))
		{
			try
			{
				P("~~~~~~~~~(3) line: " << line);
				loc.max_body = std::stoi(following_content(line, "body_size_limit"));
			}
			catch(std::exception &e)
			{
				P(e.what());
				P("Error: body size limit is not a number in .conf");
				exit(1);
			}
		}
		else if (check_line(line, "root"))
		{
			loc.root = following_content(line, "root");
			if (loc.root[0] == '/')
				loc.root =loc.root.substr(1, loc.root.size());
		}
		else if (check_line(line, "index"))
			loc.index = following_contents(line, "index");
		else if (check_line(line, "directory_listing"))
			loc.directory_listing = following_content(line, "directory_listing");
		else if (check_line(line, "default_file_if_request_directory"))
			loc.default_file_if_request_directory = following_content(line, "default_file_if_request_directory");
		else if (check_line(line, "fastcgi_param"))
			parse_cgi(line, loc, conf);
		else if (check_line(line, "file_upload_location"))
			loc.file_upload_location = following_content(line, "file_upload_location");
	}
	conf.locations.push_back(loc);
}

bool order_location_based_on_directory_lenght(const t_location &first, const t_location &second) { return (first.directory <= second.directory); }

void  parse_server(t_config &conf, std::ifstream &fd)
{
	std::string line;
	//In server block
	while (getlinecut(fd, line) && !check_line(line, "}"))
	{
		if (check_line(line, "host"))
			conf.host = following_content(line, "host");
		else if (check_line(line, "port"))
			conf.port = following_contents(line, "port");
		else if (check_line(line, "server_name"))
			conf.server_name = following_content(line, "server_name");
		else if (check_line(line, "root"))
		{
			conf.root = following_content(line, "root");
			if (conf.root[0] == '/')
				conf.root =conf.root.substr(1, conf.root.size());
		}
		else if (check_line(line, "default_error_page"))
			conf.default_error_page = following_content(line, "default_error_page");
		else if (check_line(line, "body_size_limit"))
			try
			{
				P("~~~~~~~~~(4) line:" << line);
				conf.body_size_limit = std::stoi(following_content(line, "body_size_limit"));
			}
		catch(std::exception &e)
		{
			P(e.what());
			P("Error: body size limit is not a number in .conf");
			exit(1);
		}
		else if (check_line(line, "location"))
			parse_location(conf, fd, line);
		else if (check_line(line, "index"))
			conf.index = following_contents(line, "index");
	}
	conf.locations.sort(order_location_based_on_directory_lenght); //Order locations for when searching request location link
	if (conf.port.size() == 0)
	{
		P("Error: port not specified");
		exit(1);
	}
}

void default_init(std::list<t_config> &ret)
{
	t_config conf;

	conf.host = "None";
	conf.server_name = "None";
	conf.root = "";
	conf.body_size_limit = std::string::npos; //Default max body size
	ret.push_back(conf);
}

void copy_config_with_new_port(t_config &server_block, std::string port, std::list<t_config> &c)
{
	t_config new_server_block;

	new_server_block.host = server_block.host;
	new_server_block.server_name = server_block.server_name;
	new_server_block.root = server_block.root;
	new_server_block.default_error_page = server_block.default_error_page;
	new_server_block.body_size_limit = server_block.body_size_limit;
	new_server_block.locations = server_block.locations;
	new_server_block.index = server_block.index;

	// new_server_block.s = new_server_block.s;

	new_server_block.port.push_back(port);

	c.push_back(new_server_block);
}


void one_server_multiple_ports_to_multiple_servers(t_config &server_block, std::list<t_config> &c)
{
	while(server_block.port.size() > 1)
	{
		copy_config_with_new_port(server_block, server_block.port.back(), c);
		server_block.port.pop_back();
	}
}

void one_server_block_multiple_ports(std::list<t_config> &c)
{
	for (std::list<t_config>::iterator i = c.begin(); i != c.end(); i++)
	{
		if ((*i).port.size() != 1)
			one_server_multiple_ports_to_multiple_servers(*i, c);
	}
}

void parse_config(std::string path, std::list<t_config> &ret)
{
	std::string line;
	std::ifstream fd(path);

	if (!fd.is_open())
	{
		if (errno == 2)
			P("Error: no such file or directory");
		else if (errno == 13)
			P("Error: permission denied");
		exit(1);
	}
	while (std::getline(fd, line))
	{
		if (check_line(line, "server"))
		{
			default_init(ret);
			parse_server(ret.back(), fd);
		}
	}
	fd.close();
	if (ret.size() == 0)
	{
		P("Error: server block missing");
		exit(1);
	}
	one_server_block_multiple_ports(ret);
	// show_conf(ret);
}

#include "parsing.hpp"

bool getlinecut(std::ifstream &fd, std::string &line)
{
	if (!std::getline(fd, line))
		return false;
	line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
	return true;
}

bool check_line(std::string const &line, const std::string &comp)
{
	unsigned int start = 0;
	unsigned int end = comp.size();

	if (line.size() >= start && line.substr(start, end) == comp)
		return true;
	return false;
}


std::string following_content(std::string &line, const std::string &after)
{
	if (line.size() == after.size())
		return std::string("None");
	unsigned int start = after.size() + 1;

	line = line.substr(start, line.size());
	line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
	line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
	return line;
}

std::list<std::string> split(std::string text, std::string const &sp)
{
	std::list<std::string> ret;
	std::string split;

	while(text != std::string("None"))
	{
		split = parse_until(text, sp);
		ret.push_back(split);
		text = following_content(text, split);
	}
	return ret;
}

std::list<std::string> following_contents(std::string line, const std::string &after)
{
	line = following_content(line, after);
	return split(line, " ,");
}

std::string parse_until(std::string &line, std::string const &until, bool all)
{
	unsigned int i;

	i = 0;
	while(i < line.size() && until.find(line[i]) == std::string::npos)
	{
		if (line[i] == '(') //Handle brackets
			while(line[i] != ')')
				i++;
		i++;
	}
	if (all == true && i < line.size())
		return line.substr(0, i + 1);
	return line.substr(0, i);
}

std::string parse_between(std::string &line, char cut, char cut2, bool between)
{
	unsigned int i;
	int start = 0;
	int end = 0;

	i = 0;
	while(i < line.size() && line[i] != cut)
		i++;
	if (i == line.size())
		return std::string("None");
	start = i;
	while(i < line.size() && line[i] != cut2)
		i++;
	end = i - start;
	if (between == false)
		return (line.substr(start, end + 1));
	else
		return (line.substr(start + 1, end - 1));
}

void show_locations(std::list<t_location> &locations)
{
	if (locations.size() == 0)
		return ;
	size_t i = 0;
	P("SIZE: " << locations.size());
	for (std::list<t_location>::iterator locs = locations.begin(); i < locations.size(); locs++)
	{
		i++;
		std::cout << std::string("Location: ");
		P(i);
		std::cout << "file_extensions: ";
		for (std::list<std::string>::iterator ext = locs->file_extensions.begin(); ext != locs->file_extensions.end(); ext++)
			std::cout << *ext + std::string(" ");
		std::cout << std::endl;
		std::cout << "directory: ";
		P(locs->directory);
		std::cout << "http_methods: ";
		for (std::list<std::string>::iterator http = locs->http_methods.begin(); http != locs->http_methods.end(); http++)
			std::cout << *http + std::string(" ");
		std::cout << std::endl;
		std::cout << "max_body: ";
		P(locs->max_body);
		std::cout << "root: ";
		P(locs->root);
		std::cout << "index: ";
		for (std::list<std::string>::iterator i = locs->index.begin(); i != locs->index.end(); i++)
			std::cout << *i + std::string(" ");
		std::cout << std::endl;
		std::cout << "directory_listing: ";
		P(locs->directory_listing);
		std::cout << "default_file_if_request_directory: ";
		P(locs->default_file_if_request_directory);
		std::cout << "file_upload_location: ";
		P(locs->file_upload_location);
		std::cout << "CGI: ";
		P(locs->CGI.active);
		if (locs->CGI.active)
		{
			std::cout << "AUTH_TYPE: ";
			P(locs->CGI.AUTH_TYPE);
			std::cout << "CONTENT_LENGTH: ";
			P(locs->CGI.CONTENT_LENGTH);
			std::cout << "CONTENT_TYPE: ";
			P(locs->CGI.CONTENT_TYPE);
			std::cout << "GATEWAY_INTERFACE: ";
			P(locs->CGI.GATEWAY_INTERFACE);
			std::cout << "PATH_INFO: ";
			P(locs->CGI.PATH_INFO);
			std::cout << "PATH_TRANSLATED: ";
			P(locs->CGI.PATH_TRANSLATED);
			std::cout << "QUERY_STRING: ";
			P(locs->CGI.QUERY_STRING);
			std::cout << "REMOTE_ADDR: ";
			P(locs->CGI.REMOTE_ADDR);
			std::cout << "REMOTE_INDENT: ";
			P(locs->CGI.REMOTE_INDENT);
			std::cout << "REMOTE_USER: ";
			P(locs->CGI.REMOTE_USER);
			std::cout << "REQUEST_METHOD: ";
			P(locs->CGI.REQUEST_METHOD);
			std::cout << "REQUEST_URI: ";
			P(locs->CGI.REQUEST_URI);
			std::cout << "SCRIPT_NAME: ";
			P(locs->CGI.SCRIPT_NAME);
			std::cout << "SERVER_NAME: ";
			P(locs->CGI.SERVER_NAME);
			std::cout << "SERVER_PORT: ";
			P(locs->CGI.SERVER_PORT);
			std::cout << "SERVER_PROTOCOL: ";
			P(locs->CGI.SERVER_PROTOCOL);
			std::cout << "SERVER_SOFTWARE: ";
			P(locs->CGI.SERVER_SOFTWARE);
		}
		std::cout << "URL found: ";
		P(locs->FOUND_URL);
	}
}

void show_conf(t_config &conf)
{
	P(std::string("host: ") + conf.host);
	P(std::string("port: ") + conf.port.front());
	P(std::string("server_name: ") + conf.server_name);
	P(std::string("default_error_page: ") + conf.default_error_page);
	P(std::string("body_size_limit: ") + std::to_string(conf.body_size_limit));
	std::cout << conf.index.size();
	std::cout << "Index: ";
	for (std::list<std::string>::iterator i = conf.index.begin(); i != conf.index.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	show_locations(conf.locations);
}


void show_http_request(t_http_req &req)
{
	std::cout << "Method: ";
	P(req.method);
	std::cout << "URL: ";
	P(req.URL);
	std::cout << "protocol_version: ";
	P(req.protocol_version);
	std::cout << req.header_fields.Accept_Charsets.size();
	std::cout << "Accept_Charsets: ";
	for (std::list<std::string>::iterator i = req.header_fields.Accept_Charsets.begin(); i != req.header_fields.Accept_Charsets.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Accept_Language.size();
	std::cout << "Accept_Language: ";
	for (std::list<std::string>::iterator i = req.header_fields.Accept_Language.begin(); i != req.header_fields.Accept_Language.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Allow.size();
	std::cout << "Allow: ";
	for (std::list<std::string>::iterator i = req.header_fields.Allow.begin(); i != req.header_fields.Allow.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Authorization.size();
	std::cout << "Authorization: ";
	for (std::list<std::string>::iterator i = req.header_fields.Authorization.begin(); i != req.header_fields.Authorization.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Content_Language.size();
	std::cout << "Content_Language: ";
	for (std::list<std::string>::iterator i = req.header_fields.Content_Language.begin(); i != req.header_fields.Content_Language.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Content_Length.size();
	std::cout << "Content_Length: ";
	for (std::list<std::string>::iterator i = req.header_fields.Content_Length.begin(); i != req.header_fields.Content_Length.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Content_Location.size();
	std::cout << "Content_Location: ";
	for (std::list<std::string>::iterator i = req.header_fields.Content_Location.begin(); i != req.header_fields.Content_Location.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Content_Type.size();
	std::cout << "Content_Type: ";
	for (std::list<std::string>::iterator i = req.header_fields.Content_Type.begin(); i != req.header_fields.Content_Type.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Date.size();
	std::cout << "Date: ";
	for (std::list<std::string>::iterator i = req.header_fields.Date.begin(); i != req.header_fields.Date.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Host.size();
	std::cout << "Host: ";
	for (std::list<std::string>::iterator i = req.header_fields.Host.begin(); i != req.header_fields.Host.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Last_Modified.size();
	std::cout << "Last_Modified: ";
	for (std::list<std::string>::iterator i = req.header_fields.Last_Modified.begin(); i != req.header_fields.Last_Modified.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Location.size();
	std::cout << "Location: ";
	for (std::list<std::string>::iterator i = req.header_fields.Location.begin(); i != req.header_fields.Location.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Referer.size();
	std::cout << "Referer: ";
	for (std::list<std::string>::iterator i = req.header_fields.Referer.begin(); i != req.header_fields.Referer.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Retry_After.size();
	std::cout << "Retry_After: ";
	for (std::list<std::string>::iterator i = req.header_fields.Retry_After.begin(); i != req.header_fields.Retry_After.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Server.size();
	std::cout << "Server: ";
	for (std::list<std::string>::iterator i = req.header_fields.Server.begin(); i != req.header_fields.Server.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.Transfer_Encoding.size();
	std::cout << "Transfer_Encoding: ";
	for (std::list<std::string>::iterator i = req.header_fields.Transfer_Encoding.begin(); i != req.header_fields.Transfer_Encoding.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.User_Agent.size();
	std::cout << "User_Agent: ";
	for (std::list<std::string>::iterator i = req.header_fields.User_Agent.begin(); i != req.header_fields.User_Agent.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << req.header_fields.WWW_Authenticate.size();
	std::cout << "WWW_Authenticate: ";
	for (std::list<std::string>::iterator i = req.header_fields.WWW_Authenticate.begin(); i != req.header_fields.WWW_Authenticate.end(); i++)
		std::cout << *i + std::string(" ");
	std::cout << std::endl;
	std::cout << "message_body: ";
	P(req.message_body);
}

#include "parsing.hpp"

std::string accordance_method_location(std::string url, std::string method, t_location *loc)
{
	if (url == std::string("file not found") || loc == 0)
		return url;
	for (std::list<std::string>::iterator met = loc->http_methods.begin(); met != loc->http_methods.end(); met++)
		if ((method == *met || *met == std::string("ALL")))
			return url;
	return "method not found";
}

std::string find_directory(std::string local_url, std::list<std::string> &index)
{
	std::string ret;

	if (local_url.size() != 0)
		local_url += std::string("/");
	if (index.size() != 0)
	{
		for (std::list<std::string>::iterator i = index.begin(); i != index.end(); i++) //If directory found search for given indexes
		{
			if ((ret = find_file(local_url + *i)) != std::string("file not found") && ret != std::string("directory found"))
				return ret;
		}
	}
	return "file not found";
}

std::string find_file_directory(std::string local_root, std::string directory, std::list<std::string> &index)
{
	std::string ret;
	std::string local_url;

	if (directory.size() == 0)
		local_url = local_root;
	else if (local_root.size() == 0)
		local_url = directory;
	else
		local_url = local_root + std::string("/") + directory;
	P("local_url: "<< local_url);
	if (local_url.size() == 0 || (ret = find_file(local_url)) == std::string("directory found"))
		ret = find_directory(local_url, index);
	return ret;
}

void find_in_file_extension_location(std::string &ret, std::string &last, t_location &loc, t_http_req &req, t_config &conf)
{
		if ((ret = find_file_directory(loc.root, req.URL, loc.index)) != std::string("file not found"))
		{
			for (std::list<std::string>::iterator ext = loc.file_extensions.begin(); ext != loc.file_extensions.end(); ext++)
			{
				if (get_file_extension(ret) == *ext) //If file extension is equal or all file extensions are accepted
				{
					last = ret;
					req.loc = &loc;
				}
			}
		}
}

void find_in_directory_location(std::string &ret, std::string &last, t_location &loc, t_http_req &req, t_config &conf)
{
	if ((ret = find_file_directory(loc.root, req.URL, loc.index)) != std::string("file not found"))
	{
		last = ret;
		req.loc = &loc;
	}
}

void URL_to_local_path(t_http_req &req, t_config &conf)
{
	std::string ret;
	std::string last;

	last = "file not found";
	if (req.URL.size() != 0 && req.URL[req.URL.size() - 1] == '/')
		req.URL = req.URL.substr(0, req.URL.size() - 1);
	if (req.URL.size() != 0 && req.URL[req.URL.size() - 1] == ':')
		req.URL = req.URL.substr(0, req.URL.size() - 1);
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++) //find location based on directory
	{
		P("URL: " << req.URL);
		P("DIR: " << loc->directory);
		P("COMP: " << (req.URL >= loc->directory && loc->directory != std::string("None")));
		if (req.URL >= loc->directory && loc->directory != std::string("None")) //Find in directory location
			find_in_directory_location(ret, last, *loc, req, conf);
	}
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++) //find location based on file extensions
		if (loc->file_extensions.front() != std::string("None"))
			find_in_file_extension_location(ret, last, *loc, req, conf);
	if (req.loc == 0) //Outside of any locations
		last = find_file_directory(conf.root, req.URL, conf.index);
	req.URL = last;
	req.URL = accordance_method_location(req.URL, req.method, req.loc);
}

void parse_first_line(t_http_req &req, std::string line, t_config &conf)
{
	std::list<std::string> parts;

	parts = split(line, " ");
	req.method = parts.front();
	parts.pop_front();
	req.URL = parts.front().substr(1);
	P(req.URL);
	URL_to_local_path(req, conf);
	P(req.URL);
	parts.pop_front();
	req.protocol_version = parts.front();
}

void parse_body(t_http_req &req, std::string line, t_config &conf)
{
	if (req.message_body == std::string("None"))
		req.message_body.assign(line);
	else
	{
		req.message_body.insert(req.message_body.size() - 1, "\n"); //Insert is used instead of append due to unexplained bug when using append
		req.message_body.insert(req.message_body.size() - 1, line);
	}
	if (static_cast<int>(req.message_body.size()) > conf.body_size_limit)
		req.message_body = req.message_body.substr(0, conf.body_size_limit);
}

void parse_header_fields(t_http_req &req, std::string line)
{
	if (check_line(line, "Accept-Charsets"))
		req.header_fields.Accept_Charsets = following_contents(line, "Accept-Charsets:");
	else if (check_line(line, "Accept-Language"))
			req.header_fields.Accept_Language = following_contents(line, "Accept-Language:");
	else if (check_line(line, "Allow"))
			req.header_fields.Allow = following_contents(line, "Allow:");
	else if (check_line(line, "Authorization"))
			req.header_fields.Authorization = following_contents(line, "Authorization:");
	else if (check_line(line, "Content-Language"))
			req.header_fields.Content_Language = following_contents(line, "Content-Language:");
	else if (check_line(line, "Content-Length"))
			req.header_fields.Content_Length = following_contents(line, "Content-Length:");
	else if (check_line(line, "Content-Location"))
			req.header_fields.Content_Location = following_contents(line, "Content-Location:");
	else if (check_line(line, "Content-Type"))
			req.header_fields.Content_Type = following_contents(line, "Content-Type:");
	else if (check_line(line, "Date"))
			req.header_fields.Date = following_contents(line, "Date:");
	else if (check_line(line, "Host"))
			req.header_fields.Host = following_contents(line, "Host:");
	else if (check_line(line, "Last-Modified"))
			req.header_fields.Last_Modified = following_contents(line, "Last-Modified:");
	else if (check_line(line, "Location"))
			req.header_fields.Location = following_contents(line, "Location:");
	else if (check_line(line, "Referer"))
			req.header_fields.Referer = following_contents(line, "Referer:");
	else if (check_line(line, "Retry-After"))
			req.header_fields.Retry_After = following_contents(line, "Retry-After:");
	else if (check_line(line, "Server"))
			req.header_fields.Server = following_contents(line, "Server:");
	else if (check_line(line, "Transfer-Encoding"))
			req.header_fields.Transfer_Encoding = following_contents(line, "Transfer-Encoding:");
	else if (check_line(line, "User-Agent"))
			req.header_fields.User_Agent = following_contents(line, "User-Agent:");
	else if (check_line(line, "WWW_Authenticate"))
			req.header_fields.WWW_Authenticate = following_contents(line, "WWW_Authenticate:");
}

void parse(t_http_req &req, std::list<std::string> lines, unsigned int body_line, t_config &conf)
{
	unsigned int i;

	i = 0;
	for (std::list<std::string>::iterator line = lines.begin(); line != lines.end(); line++)
	{
		if (i == 0)
			parse_first_line(req, *line, conf);
		else if (i >= body_line)
			parse_body(req, *line, conf);
		else
			parse_header_fields(req, *line);
		i++;
	}
}

unsigned int find_body(std::string req)
{
	unsigned int counter;
	bool follow;

	counter = 0;
	follow = false;
	for (unsigned int i = 0; i < req.size() ; i++)
	{
		if (req[i] == '\n')
		{
			if (follow == true) //Following \n found meaning empty line found meaning next line is body
				break ;
			counter++;
		}
		if (req[i] == '\n')
			follow = true;
		else if (req[i] != '\r')
			follow = false;
	}
	return counter; //Returns line number of body
}

void default_init(t_http_req &req)
{
	req.header_fields.Accept_Charsets.push_back("None");
	req.header_fields.Accept_Language.push_back("None");
	req.header_fields.Allow.push_back("None");
	req.header_fields.Authorization.push_back("None");
	req.header_fields.Content_Language.push_back("None");
	req.header_fields.Content_Length.push_back("None");
	req.header_fields.Content_Location.push_back("None");
	req.header_fields.Content_Type.push_back("None");
	req.header_fields.Date.push_back("None");
	req.header_fields.Host.push_back("None");
	req.header_fields.Last_Modified.push_back("None");
	req.header_fields.Location.push_back("None");
	req.header_fields.Referer.push_back("None");
	req.header_fields.Retry_After.push_back("None");
	req.header_fields.Server.push_back("None");
	req.header_fields.Transfer_Encoding.push_back("None");
	req.header_fields.User_Agent.push_back("None");
	req.header_fields.WWW_Authenticate.push_back("None");
	req.method = std::string("None");
	req.URL = std::string("None");
	req.loc = 0;
	req.protocol_version = std::string("None");
	req.message_body = std::string("None");
}

t_http_req parse_http_request(std::string req, t_config &conf)
{
	t_http_req ret;// = new t_http_req;
	std::list<std::string> lines;
	unsigned int body_line;

	/*P("--------------------------------------------------------------------------");
	P("REAL REQUEST:");
	P(req); //test
	P("--------------------------------------------------------------------------");
	*/body_line = find_body(req);
	lines = split(req, "\n");
	default_init(ret);
	parse(ret, lines, body_line, conf);
	P("--------------------------------------------------------------------------");
	P("PARSED REQUEST:");
	show_http_request(ret); //test
	P("--------------------------------------------------------------------------");
	return (ret);
}

#include "parsing.hpp"

void parse_first_line(t_http_req &req, std::string line)
{
	std::list<std::string> parts;

	parts = split(line, " ");
	req.method = parts.front();
	parts.pop_front();
	req.URL = parts.front();
	parts.pop_front();
	req.protocol_version = parts.front();
}

void parse_body(t_http_req &req, std::string line)
{
	if (req.message_body == std::string("None"))
		req.message_body.assign(line);
	else
	{
		req.message_body.insert(req.message_body.size() - 1, "\n"); //Insert is used instead of append due to unexplained bug when using append
		req.message_body.insert(req.message_body.size() - 1, line);
	}
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

void parse(t_http_req &req, std::list<std::string> lines, unsigned int body_line)
{
	unsigned int i;

	i = 0;
	for (std::list<std::string>::iterator line = lines.begin(); line != lines.end(); line++)
	{
		if (i == 0)
			parse_first_line(req, *line);
		else if (i >= body_line)
			parse_body(req, *line);
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
	req.protocol_version = std::string("None");
	req.message_body = std::string("None");
}

t_http_req *parse_http_request(std::string req)
{
	t_http_req *ret = new t_http_req;
	std::list<std::string> lines;
	unsigned int body_line;

	P("--------------------------------------------------------------------------");
	P(req); //test
	P("--------------------------------------------------------------------------");
	body_line = find_body(req);
	lines = split(req, "\n");
	default_init(*ret);
	parse(*ret, lines, body_line);
	P("--------------------------------------------------------------------------");
	show_http_request(*ret); //test
	P("--------------------------------------------------------------------------");
	return (ret);
}

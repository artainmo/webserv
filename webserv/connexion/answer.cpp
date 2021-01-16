#include "connexion.hpp"
#include "answer.hpp"

std::string header_line(std::string protocol_version, std::string code, std::string text)
{
  return protocol_version + std::string(" ") + code + std::string(" ") + text;
}

std::string error_page()
{
  std::ifstream fd("default/error.html");
  std::string 	file("\n");
  std::string 	line;

  if (!fd.is_open())
  {
    std::cout << "Error: file opening default/error.html" << std::endl;
    exit(1);
  }
  while (getline(fd, line))
    file += line;
  return header_line("HTTP/1.1", "400", "BAD REQUEST") + file;
}

std::string get_date()
{
	char			buf[255];
	struct tm		timeinfo;
	time_t			time;
	struct timeval	current_time;
	
	gettimeofday(&current_time, NULL);
	time = (time_t)current_time.tv_sec;
	strptime(ctime(&time), "%a %b %d %T %Y", &timeinfo);
	strftime(buf, 255, "%a, %d %b %Y %T GMT", &timeinfo);
	
	return std::string(buf);
}

std::string get_last_modified(std::string const& filename)
{
	char		buf[255];
	struct tm	timeinfo;
	struct stat	file;

	stat(filename.c_str(), &file);
	strptime(ctime(&file.st_mtime), "%a %b %d %T %Y", &timeinfo);
	strftime(buf, 255, "%a, %d %b %Y %T GMT", &timeinfo);

	return std::string(buf);
}

std::string get_content_type(std::string const& filename)
{
	int			i;
	std::string	filetype;

	for (i = filename.size() - 1; i > 0; i--)
	{
		if (filename[i] == '.')
			break ;
	}
	filetype = std::string(filename, i + 1, filename.size() - 1);

	if (filetype == "html" || filetype == "php")
		return (std::string("text/html"));
	else if (filetype == "png")
		return (std::string("image/png"));
	else if (filetype == "css")
		return (std::string("text/css"));
	else if (filetype == "jpg")
		return (std::string("image/webp"));
	return std::string("text/html");
}

void	init_response_struct(t_answer_headers & response)
{
	response.server = std::string("Server: ");
	response.date = std::string("Date: ");
	response.content_type = std::string("Content-Type: ");
	response.content_length = std::string("Content-Length: ");
	response.last_modified = std::string("Last-Modified: ");
}

std::string construct_header_and_send(t_answer_headers const& info)
{
	std::string response;

	response = info.header_line + "\n"
			+ info.server + "\n"
			+ info.date + "\n"
			+ info.content_type + "\n"
			+ info.content_length + "\n"
			+ info.last_modified + "\n"
			+ "\n"
			+ info.body + "\r";
	return response;
}

std::string GET(std::string path)
{
	std::ifstream		fd(path);
	std::string			file;
	std::string			line;
	t_answer_headers	response;
	struct stat			stat_file;

	if (!fd.is_open())
		return error_page();
	stat(path.c_str(), &stat_file);
	while (getline(fd, line))
	{
		file += line + "\n";
	//	std::cout << "line= " << line << std::endl;
	}
	
	init_response_struct(response);
	response.server += std::string("webserv/1.0");
	response.date += get_date();
	response.last_modified += get_last_modified(path);
	response.content_length += std::to_string(file.size());
	response.header_line += header_line("HTTP/1.1", "200", "OK");
	response.body = file;
	response.content_type += get_content_type(path);
std::cout << "line= " << response.body << std::endl;
	return construct_header_and_send(response);
}

std::string parse_method(t_server &s, t_http_req &req)
{ 
	if (req.method == std::string("GET"))
	{
		std::string	path = "../tests";
		path += req.URL;
		//if (req.URL[req.URL.size() - 1] )    MANAGE ThE  '/' at the end 
		path += "index.html";  // need to check first index.html or other files depends from the config, better use fstat
		return GET(path);
	}
	if (req.method == std::string("HEAD"))
		return header_line("HTTP/1.1", "200", "OK") + std::string("\n");
	else if (req.method == std::string("POST"))
		pass;
	else if (req.method == std::string("PUT"))
		pass;
	else if (req.method == std::string("DELETE"))
		pass;
	else if (req.method == std::string("CONNECT"))
		pass;
	else if (req.method == std::string("OPTIONS"))
		pass;
	else if (req.method == std::string("TRACE"))
		pass;
	(void)s;
	return error_page();
}

void answer_http_request(t_server &s, t_http_req &req)
{
	std::string	answer;

	answer = parse_method(s, req);
	if (send(s.socket_to_answer, answer.c_str(), answer.size(), 0) == -1)
	{
		std::cout << "Error: send failed" << std::endl;
		exit(1);
	}
}

#include "connexion.hpp"

std::string header_line(std::string protocol_version, std::string code, std::string text)
{
  return protocol_version + std::string(" ") + code + std::string(" ") + text + std::string("\r\n");
}

std::string header_field(std::string header, std::string text)
{
  return header + std::string(": ") + text + std::string("\r\n");
}

std::string get_header_line(size_t const& number)
{
	std::string	protocol("HTTP/1.1 ");

	switch (number)
	{
		case 200:
			return (protocol + std::string("200 OK"));
		case 404:
			return (protocol + std::string("404 Not Found"));
		case 405:
			return (protocol + std::string("405 Not Allowed"));
    case 400:
      return (protocol + std::string("400 Bad Request"));
		default:
			return (protocol + std::string("404 Not Found"));
	}
	return (protocol + std::string("404 Not Found"));
}

std::string get_date(void)
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
	std::string	filetype;

	filetype = get_file_extension(filename);
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

void	fill_response_struct(t_answer_headers & response, std::string const& file, std::string const& path, size_t const& response_number)
{
	response.server += std::string("webserv/1.0");
	response.date += get_date();
	response.last_modified += get_last_modified(path);
	response.content_length += std::to_string(file.size());
	response.header_line += get_header_line(response_number);
	response.body = file;
	response.content_type += get_content_type(path);
}

std::string construct_get_response(t_answer_headers const& info)
{
	std::string response;

	response = info.header_line + "\r\n"
			+ info.server + "\r\n"
			+ info.date + "\r\n"
			+ info.content_type + "\r\n"
			+ info.content_length + "\r\n"
			+ info.last_modified + "\r\n"
			+ "\r\n"
			+ info.body;
	return response;
}

std::string construct_head_response(t_answer_headers const& info)
{
	std::string response;

	response = info.header_line + "\r\n"
			+ info.server + "\r\n"
			+ info.date + "\r\n"
			+ info.content_type + "\r\n"
			+ info.content_length + "\r\n"
			+ info.last_modified + "\r\n"
			+ "\r\n";
	return response;
}

std::string construct_error_response(t_answer_headers const& info)
{
	std::string response;

	response = info.header_line + "\r\n"
			+ info.server + "\r\n"
			+ info.date + "\r\n"
			+ info.content_type + "\r\n"
			+ info.content_length + "\r\n"   ////////////////////// NOT GOUD BUT I DON'T KNOW WHY
			+ "\r\n"
			+ info.body;
	return response;
}

void init_head_get(std::string const& path, std::ifstream & fd, t_answer_headers & response, size_t const& response_number)
{
	std::string			file;
	std::string			line;
	struct stat			stat_file;

	stat(path.c_str(), &stat_file);
	while (getline(fd, line))
		file += line + "\n";
	init_response_struct(response);
	fill_response_struct(response, file, path, response_number);
}

std::string error_page(size_t error_nbr)
{
	std::string			path("../default/" + std::to_string(error_nbr) + ".html");
	std::ifstream 		fd(path);  // charger l'erreur
	t_answer_headers	info;

	if (!fd.is_open())
	{
		std::cout << "Error: file opening ../default/error.html" << std::endl;
		exit(1);
	}
	init_head_get(path, fd, info, error_nbr);
	return construct_error_response(info);
}

std::string GET(t_http_req &req)
{
	std::ifstream		fd;
	t_answer_headers	response;

  if (req.loc != 0 && req.loc->CGI != 0)
	   req.URL = get_cgi(req);
  fd.open(req.URL);
	init_head_get(req.URL, fd, response, 200);
	fd.close();
	return construct_get_response(response);
}

std::string HEAD(std::string path)
{
	std::ifstream		fd;
	t_answer_headers	response;

	fd.open(path);
	init_head_get(path, fd, response, 200);
	fd.close();
	return construct_head_response(response);
}

std::string POST(t_http_req &req)
{
	std::ifstream		fd;

	return GET(req);
	// return header_line("HTTP/1.1", "206", "Partial Content");
	//
  // return header_line("HTTP/1.1", "304", "Not Modified");
	//
  // return header_line("HTTP/1.1", "416", "Range Not Satisfiable");
	//
  // return header_line("HTTP/1.1", "201", "Created") + header_field("Location: ", path);
}

std::string parse_method(t_http_req &req, t_config &conf)
{
	if (req.method == std::string("GET"))
		return GET(req);
	else if (req.method == std::string("HEAD"))
		return HEAD(req.URL);
	else if (req.method == std::string("POST"))
		return POST(req);
	else if (req.method == std::string("PUT"))
		return error_page(405);
	else if (req.method == std::string("DELETE"))
		return error_page(405);
	return error_page(404);
}

void answer_http_request(int socket_to_answer, t_http_req &req, t_config &conf, t_server &s)
{
	std::string	answer;

  if (req.error == true)
    answer = error_page(400);
  if (req.URL == std::string("file not found"))
    answer = error_page(404);
  else if (req.URL == std::string("method not found"))
    answer = error_page(405);
	else
    answer = parse_method(req, conf);
  if (FD_ISSET(socket_to_answer , &s.active_socket_write)) //If socket still in active write sockets, the socket is writable
  {
    P("YES");
	 if (send(socket_to_answer, answer.c_str(), answer.size(), 0) == -1)
	 {
		  std::cout << "Error: send failed" << std::endl;
		   // exit(1);
	 }
  }
}

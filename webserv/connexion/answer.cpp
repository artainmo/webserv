#include "connexion.hpp"


std::string header_line(std::string protocol_version, std::string code, std::string text)
{
  return protocol_version + std::string(" ") + code + std::string(" ") + text + std::string("\r\n");
}

std::string header_field(std::string header, std::string text)
{
  return header + std::string(": ") + text + std::string("\r\n");
}

std::string get_header_line(int const& number)
{
	std::string	protocol("HTTP/1.1 ");

	switch (number)
	{
		case 200:
			return (protocol + std::string("200 OK"));
		case 201:
			return (protocol + std::string("201 Created"));
		case 404:
			return (protocol + std::string("404 Not Found"));
		case 405:
			return (protocol + std::string("405 Not Allowed"));
    	case 400:
      		return (protocol + std::string("400 Bad Request"));
    	case 416:
          return (protocol + std::string("416 Range Not Satisfiable"));
		case 500:
      		return (protocol + std::string("500 Internal Server Error"));
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

void	init_response_struct(t_header_fields & response)
{
	response.Server.push_front(std::string("Server: "));
	response.Date.push_front(std::string("Date: "));
	response.Content_Type.push_front(std::string("Content-Type: "));
	response.Content_Length.push_front(std::string("Content-Length: "));
	response.Last_Modified.push_front(std::string("Last-Modified: "));
	response.Location.push_front(std::string("Location: "));
}

void	fill_response_struct(t_header_fields & response, std::string const& file, std::string const& path, int const& response_number)
{
	response.Server.front() += std::string("webserv/1.0");
	response.Date.front() += get_date();
	response.Last_Modified.front() += get_last_modified(path);
	response.Content_Length.front() += std::to_string(file.size());
	response.Header_Line.front() += get_header_line(response_number);
	response.Body.front() = file;
	response.Content_Type.front() += get_content_type(path);
	response.Location.front() += path;
}

std::string construct_get_response(t_header_fields const& info)
{
	std::string response;

	response = info.Header_Line.front() + "\r\n"
		+ info.Server.front() + "\r\n"
		+ info.Date.front() + "\r\n"
		+ info.Content_Type.front() + "\r\n"
		+ info.Content_Length.front() + "\r\n"
		+ info.Last_Modified.front() + "\r\n"
		+ "\r\n"
		+ info.Body.front();
	return response;
}

std::string construct_head_response(t_header_fields const& info)
{
	std::string response;

	response = info.Header_Line.front() + "\r\n"
		+ info.Server.front() + "\r\n"
		+ info.Date.front() + "\r\n"
		+ info.Content_Type.front() + "\r\n"
		+ info.Content_Length.front() + "\r\n"
		+ info.Last_Modified.front() + "\r\n"
		+ "\r\n";
	return response;
}

std::string construct_put_response(t_header_fields const& info)
{
	std::string response;

  response = info.Header_Line.front() + "\r\n"
		+ info.Server.front() + "\r\n"
		+ info.Date.front() + "\r\n"
		+ info.Location.front() + "\r\n"
		+ "\r\n";
	return response;
}

std::string construct_error_response(t_header_fields const& info, std::string methode)
{
	std::string response;

	response = info.Header_Line.front() + "\r\n"
		+ info.Server.front() + "\r\n"
		+ info.Date.front() + "\r\n"
		+ info.Content_Type.front() + "\r\n"
		+ info.Content_Length.front() + "\r\n"   ////////////////////// NOT GOUD BUT I DON'T KNOW WHY
		+ "\r\n";
	if (methode != "HEAD")
		response += info.Body.front();
	return response;
}

void init_head_get(std::string const& path, std::ifstream & fd, t_header_fields & response, int const& response_number)
{
	std::string			file;
	std::string			line;

	while (getline(fd, line))
		file += line + "\n";
	init_response_struct(response);
	fill_response_struct(response, file, path, response_number);
}

void init_put(std::string const& path, std::ofstream & fd, t_header_fields & response, int const& response_number)
{
	std::string			file;

	init_response_struct(response);
	fill_response_struct(response, file, path, response_number);
}

std::string error_page(int error_nbr, std::string methode)
{
	std::string			path("../default/" + std::to_string(error_nbr) + ".html");
	std::ifstream 		fd(path);  // charger l'erreur
	t_header_fields	info;

	if (!fd.is_open())
	{
		std::cout << "Error: file opening ../default/error.html" << std::endl;
		exit(1);
	}
	init_head_get(path, fd, info, error_nbr);
	return construct_error_response(info, methode);
}

std::string GET(t_http_req &req, t_config &conf, t_server &s)
{
	std::ifstream		fd;
	t_header_fields	response;

  fd.open(req.URL);
  if (!fd.is_open())
    return error_page(404, req.method);
  if (req.loc != 0 && req.loc->CGI != 0)
  	 req.URL = get_cgi(req, conf, s);
	init_head_get(req.URL, fd, response, 200);
	fd.close();
	return construct_get_response(response);
}

std::string HEAD(std::string path) // Ne devrait pas fonctionner
{
	std::ifstream		fd;
	t_header_fields	response;

	fd.open(path);
	init_head_get(path, fd, response, 200);
	fd.close();
	return construct_head_response(response);
}

std::string POST(t_http_req &req, t_config &conf, t_server &s)
{
	std::ifstream		fd;

  P("POST");
	return GET(req, conf, s);
	// return header_line("HTTP/1.1", "206", "Partial Content");
	//
  // return header_line("HTTP/1.1", "304", "Not Modified");
	//
  // return header_line("HTTP/1.1", "416", "Range Not Satisfiable");
	//
  // return header_line("HTTP/1.1", "201", "Created") + header_field("Location: ", path);
}

std::string PUT(t_http_req &req)
{
	std::ofstream			fd;
	t_header_fields		response;
	int						status_code;

	req.URL = req.loc->file_upload_location + std::string("/") + final_file_in_path(req.URL);
	if (file_exists(req.URL))
		status_code = 200; //OK (file already existed)
	else
		status_code = 201; //CREATED (new file)
  fd.open(req.URL,  std::ofstream::out | std::ofstream::trunc); // Create the file or delete it if already exist
  if (!fd.is_open())
  {
    P("Error: Put file opening");
    P(strerror(errno));
    P(req.URL);
    return error_page(404, req.method); // CHANGE THE ERROR CODE?
  }
  //write_put_file(fd,req.message_body);
  fd << req.message_body;
	init_put(req.URL, fd, response, status_code);
	fd.close();
	return construct_put_response(response);
}

std::string parse_method(t_http_req &req, t_config &conf, t_server &s)
{
	if (req.method == std::string("GET"))
		return GET(req, conf, s);
	else if (req.method == std::string("HEAD"))
		return HEAD(req.URL);
	else if (req.method == std::string("POST"))
		return POST(req, conf, s);
	else if (req.method == std::string("PUT"))
		return PUT(req);
	else if (req.method == std::string("DELETE"))
		return error_page(405, req.method);
	return error_page(404, req.method);
}

void socket_erase(std::map<int, std::string>::iterator &socket, t_server &s)
{
	int rem;

	std::cout << "Erasing socket: "<< socket->first << std::endl;
	rem = socket->first;
	socket++;
	s.requests.erase(rem);
}

void get_answer(std::map<int, std::string>::iterator &socket, t_http_req &req, t_config &conf, t_server &s)
{
	std::string	answer;

  P("URL" << req.URL);
  P("method" << req.method);
  P("ERROR" << req.error);
  if (req.error == true)
    answer = error_page(400, req.method); //Do nothing and consider the request as wrong
  else if (req.URL == std::string("file not found"))
    answer = error_page(404, req.method);
  else if (req.URL == std::string("method not found"))
    answer = error_page(405, req.method);
	else
    answer = parse_method(req, conf, s);
	s.answer[socket->first] = answer;
  socket_erase(socket, s);
}

#include "connexion.hpp"

void client_disconnection(t_server &s, unsigned int i);

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

void	init_response_struct(t_answer_headers & response)
{
	response.server = std::string("Server: ");
	response.date = std::string("Date: ");
	response.content_type = std::string("Content-Type: ");
	response.content_length = std::string("Content-Length: ");
	response.last_modified = std::string("Last-Modified: ");
	response.location = std::string("Location: ");
}

void	fill_response_struct(t_answer_headers & response, std::string const& file, std::string const& path, int const& response_number)
{
	response.server += std::string("webserv/1.0");
	response.date += get_date();
	response.last_modified += get_last_modified(path);
	response.content_length += std::to_string(file.size());
	response.header_line += get_header_line(response_number);
	response.body = file;
	response.content_type += get_content_type(path);
	response.location += path;
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

std::string construct_put_response(t_answer_headers const& info)
{
	std::string response;

	response = info.header_line + "\r\n"
			+ info.server + "\r\n"
			+ info.date + "\r\n"
		//	+ info.content_type + "\r\n"
		//	+ info.content_length + "\r\n"
		//	+ info.last_modified + "\r\n"
			+ info.location + "\r\n"
			+ "\r\n";
	return response;
}

std::string construct_error_response(t_answer_headers const& info, std::string methode)
{
	std::string response;

	std::cout << "METHOD: " << methode << std::endl;
	response = info.header_line + "\r\n"
			+ info.server + "\r\n"
			+ info.date + "\r\n"
			+ info.content_type + "\r\n"
			+ info.content_length + "\r\n"   ////////////////////// NOT GOUD BUT I DON'T KNOW WHY
			+ "\r\n";
	if (methode != "HEAD")
		response += info.body;
	return response;
}

void init_head_get(std::string const& path, std::ifstream & fd, t_answer_headers & response, int const& response_number)
{
	std::string			file;
	std::string			line;

	while (getline(fd, line))
		file += line + "\n";
	init_response_struct(response);
	fill_response_struct(response, file, path, response_number);
}

void init_put(std::string const& path, std::ofstream & fd, t_answer_headers & response, int const& response_number)
{
	std::string			file;

	init_response_struct(response);
	fill_response_struct(response, file, path, response_number);
}

std::string error_page(int error_nbr, std::string methode)
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
	return construct_error_response(info, methode);
}

std::string GET(t_http_req &req)
{
	std::ifstream		fd;
	t_answer_headers	response;

  if (req.loc != 0 && req.loc->CGI != 0)
	   req.URL = get_cgi(req);
  fd.open(req.URL);
  if (!fd.is_open())
    return error_page(500, req.method);
	init_head_get(req.URL, fd, response, 200);
	fd.close();
	return construct_get_response(response);
}

std::string HEAD(std::string path) // Ne devrait pas fonctionner
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

void    write_put_file(std::ofstream & fd, std::string message_body)
{
    std::list<std::string>  line_of_body;
    std::string             new_body;
    std::string             line;
    line_of_body = split(message_body, "\n");
    while (line_of_body.size() > 0)
    {
        if (line_of_body.front()[0] == '0')
            break ;
        line_of_body.pop_front();
        line = line_of_body.front();
        new_body += std::string(line, 0, line.size() - 1);
        line_of_body.pop_front();
    }
    fd << new_body;
}

std::string PUT(t_http_req &req)
{
	std::ofstream			fd;
	t_answer_headers		response;
	int						status_code;
  /*if (req.loc != 0 && req.loc->CGI != 0)
	   req.URL = get_cgi(req);*/
	req.URL = req.loc->file_upload_location + std::string("/") + final_file_in_path(req.URL);
	if (file_exists(req.URL))
		status_code = 200; //OK (file already existed)
	else
		status_code = 201; //CREATED (new file)
  fd.open(req. URL.c_str(),  std::ofstream::out | std::ofstream::trunc); // Create the file or delete it if already exist
  if (!fd.is_open())
    return error_page(500, req.method); // CHANGE THE ERROR CODE?
  write_put_file(fd,req.message_body);
	init_put(req.URL, fd, response, status_code);
	fd.close();
	return construct_put_response(response);
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
	s.socket_to_answer.erase(rem);
}

void get_answer(std::map<int, std::string>::iterator &socket, t_http_req &req, t_config &conf, t_server &s)
{
	std::string	answer;

  if (req.error == true)
    answer = error_page(400, req.method); //Do nothing and consider the request as wrong
  else if (req.URL == std::string("file not found") && req.method != "PUT")
    answer = error_page(404, req.method);
  else if (req.URL == std::string("method not found"))
    answer = error_page(405, req.method);
	else
    answer = parse_method(req, conf);
	s.answer_to_send[socket->first] = answer;
  socket_erase(socket, s);
}

#include "connexion.hpp"

t_location *get_location(std::string file_extension, std::string method, t_config &conf)
{
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++)
		for (std::list<std::string>::iterator ext = loc->file_extensions.begin(); ext != loc->file_extensions.end(); ext++)
				if ((file_extension == *ext || *ext == std::string("ALL")) && loc->CGI != 0)
					for (std::list<std::string>::iterator met = loc->http_methods.begin(); met != loc->http_methods.end(); met++)
						if ((method == *met || *met == std::string("ALL")) && loc->CGI != 0)
							return &(*loc);
	return 0;
}

void set_env(std::string var, std::string equal_to)
{
	pid_t	pid;

	if ((pid = fork()) == -1)
	{
		P("Error: fork failed");
		exit(1);
	}
	if (!pid)
	{
		execve("/bin/export", (char *[]) {(char *)(var + std::string("=") + equal_to).c_str(), 0}, 0);
		exit(1);
	}
	waitpid(pid, 0, 0);
}

void set_meta_variables(t_CGI &c)
{
	set_env("AUTH_TYPE", c.AUTH_TYPE);
	set_env("CONTENT_LENGTH", c.CONTENT_LENGTH);
	set_env("CONTENT_TYPE", c.CONTENT_TYPE);
	set_env("GATEWAY_INTERFACE", c.GATEWAY_INTERFACE);
	set_env("PATH_INFO", c.PATH_INFO);
	set_env("PATH_TRANSLATED", c.PATH_TRANSLATED);
	set_env("QUERY_STRING", c.QUERY_STRING);
	set_env("REMOTE_ADDR", c.REMOTE_ADDR);
	set_env("REMOTE_INDENT", c.REMOTE_INDENT);
	set_env("REMOTE_USER", c.REMOTE_USER);
	set_env("REQUEST_METHOD", c.REQUEST_METHOD);
	set_env("REQUEST_URI", c.REQUEST_URI);
	set_env("SCRIPT_NAME", c.SCRIPT_NAME);
	set_env("SERVER_NAME", c.SERVER_NAME);
	set_env("SERVER_PORT", c.SERVER_PORT);
	set_env("SERVER_PROTOCOL", c.SERVER_PROTOCOL);
	set_env("SERVER_SOFTWARE", c.SERVER_SOFTWARE);
}

void write_to_upload_file(int fd_upload_location, std::string path)
{
	int fd_1;
	pid_t pid;

	fd_1 = dup(1);
	if ((pid = fork()) == -1)
	{
		P("Error: fork failed");
		exit(1);
	}
	dup2(fd_upload_location, 1); //dup the execve output towards the upload location file, php will generate an HTML that can be send back
	if (!pid)
	{
		if (execve("/usr/bin/php", (char *[]) {(char *)"php", (char *)path.c_str(), 0}, 0) == -1) //brew install php, use php as CGI script, output file in upload location
		{
			P("Error: execve cgi php");
			P(strerror(errno));
			exit(1);
		}
		exit(1);
	}
	waitpid(pid, 0, 0);
	dup2(fd_1, 1);
	close(fd_1);
	close(fd_upload_location);
}

std::string get_cgi(t_http_req &req) //returns false if cgi not used and returns true if cgi used and file_upload_location should be used
{
	int fd_upload_location;
	std::string generated_file_path;

	// if ((loc = get_location(get_file_extension(path), method, conf)) == 0) //cgi must not be used for this file extension
	// 	return std::string("None");
	set_meta_variables(*req.loc->CGI);
	generated_file_path = req.loc->root + req.loc->file_upload_location;
	if ((fd_upload_location = open(generated_file_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
	{
		P("Error: file upload location error");
		P(strerror(errno));
		exit(1);
	}
	write_to_upload_file(fd_upload_location, req.URL);
	return generated_file_path;
}

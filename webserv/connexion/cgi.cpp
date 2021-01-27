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

void set_env(std::string var, std::string equal_to, std::vector<std::string> &vec) //Maybe avoid creating env variables in another process
{
	vec.push_back(var + "=" + equal_to);
	// pid_t	pid;

	// if ((pid = fork()) == -1)
	// {
	// 	P("Error: fork failed");
	// 	exit(1);
	// }
	// if (!pid)
	// {
	// 	execve("/bin/export", (char *[]) {(char *)(var + std::string("=") + equal_to).c_str(), 0}, 0);
	// 	exit(1);
	// }
	// waitpid(pid, 0, 0);
}

void env_meta_variables(t_CGI &c, std::vector<std::string> &vec_env) //env variables is correct
{
	set_env("AUTH_TYPE", c.AUTH_TYPE, vec_env);
	set_env("CONTENT_LENGTH", c.CONTENT_LENGTH, vec_env);
	set_env("CONTENT_TYPE", c.CONTENT_TYPE, vec_env);
	set_env("GATEWAY_INTERFACE", c.GATEWAY_INTERFACE, vec_env);
	set_env("PATH_INFO", c.PATH_INFO, vec_env); //req.URL
	set_env("PATH_TRANSLATED", c.PATH_TRANSLATED, vec_env);
	set_env("QUERY_STRING", c.QUERY_STRING, vec_env);
	set_env("REMOTE_ADDR", c.REMOTE_ADDR, vec_env);
	set_env("REMOTE_INDENT", c.REMOTE_INDENT, vec_env);
	set_env("REMOTE_USER", c.REMOTE_USER, vec_env);
	set_env("REQUEST_METHOD", c.REQUEST_METHOD, vec_env); //env variable -> POST/GET/....
	set_env("REQUEST_URI", c.REQUEST_URI, vec_env);
	set_env("SCRIPT_NAME", c.SCRIPT_NAME, vec_env);
	set_env("SERVER_NAME", c.SERVER_NAME, vec_env);
	set_env("SERVER_PORT", c.SERVER_PORT, vec_env);
	set_env("SERVER_PROTOCOL", c.SERVER_PROTOCOL, vec_env); //HTTP/1.1
	set_env("SERVER_SOFTWARE", c.SERVER_SOFTWARE, vec_env);
}

void set_meta_variables(t_CGI &c, t_http_req &req, t_config &conf, t_server &s, std::vector<std::string> &vec_env)
{
	c.CONTENT_LENGTH = req.message_body.size(); //size of the body
	c.CONTENT_TYPE = req.header_fields.Content_Type.front();
	// c->GATEWAY_INTERFACE = std::string("None");
	c.PATH_INFO = req.URL; //Should be init URL
	c.PATH_TRANSLATED = req.URL; //Should be local URL
	// c->QUERY_STRING = std::string(""); //Default empty
	c.REMOTE_ADDR = std::to_string(s.address.sin_addr.s_addr);
	// c->REMOTE_INDENT = std::string("None");
	c.REMOTE_USER = req.header_fields.Host.front();
	c.REQUEST_METHOD = req.method;
	c.REQUEST_URI = req.URL;
	// c.SCRIPT_NAME = std::string("None"); //Already init
	c.SERVER_NAME = conf.server_name;
	c.SERVER_PROTOCOL = req.protocol_version;
	c.SERVER_SOFTWARE = std::string("None");
	env_meta_variables(c, vec_env);
}

// void unset_metavariables ?? Unset automatically when process dies?

void write_to_upload_file(int &fd_upload_location, t_http_req &req, std::vector<std::string> vec_env)
{
	pid_t pid;
	std::string executable = "/usr/bin/php";

	if (req.loc->CGI->PATH_INFO != std::string("None"))
	 	executable = req.loc->CGI->SCRIPT_NAME;
	if ((pid = fork()) == -1)
	{
		P("Error: fork failed");
		exit(1);
	}
	//dup2(fd_upload_location, 1);
	if (!pid)
	{
		char *tab_env[vec_env.size() + 1];
		char *tab_execve[3];

		tab_env[vec_env.size()] = nullptr;
		for (size_t i = 0; i < vec_env.size(); i++)
			tab_env[i] = (char*)vec_env[i].c_str();	
		tab_execve[0] = (char*)executable.c_str();
		tab_execve[1] = (char*)req.URL.c_str();

		std::cout << "exec: " << tab_execve[0] << " file: " << tab_execve[1] << std::endl;
		
		tab_execve[2] = nullptr;
		dup2(fd_upload_location, 1);
		if (execve(tab_execve[0], tab_execve, tab_env) == -1)
		{
			P("Error: execve cgi php");
			P(strerror(errno));
			exit(1);
		}
		exit(1);
	}
	waitpid(pid, 0, 0);

}

std::string get_cgi(t_http_req &req, t_config &conf, t_server &s) //returns false if cgi not used and returns true if cgi used and file_upload_location should be used
{
	int fd_upload_location;
	std::vector<std::string> vec_env;
	std::string generated_file_path;

	// if ((loc = get_location(get_file_extension(path), method, conf)) == 0) //cgi must not be used for this file extension
	// 	return std::string("None");
	set_meta_variables(*req.loc->CGI, req, conf, s, vec_env);
	generated_file_path = req.loc->root + req.loc->file_upload_location;
	//P("UPLOAD:" << generated_file_path);
	if ((fd_upload_location = open(generated_file_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
	{
		P("Error: file upload location error");
		P(strerror(errno));
		exit(1);
	}
	write_to_upload_file(fd_upload_location, req, vec_env);
	close(fd_upload_location);
	return generated_file_path;
}

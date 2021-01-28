#include "connexion.hpp"

void set_env(std::string var, std::string equal_to, std::vector<std::string> &vec) //Maybe avoid creating env variables in another process
{
	vec.push_back(var + "=" + equal_to);
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

//	show_cgi(vec_env);
}

void set_meta_variables(t_CGI &c, t_http_req &req, t_config &conf, t_server &s, std::vector<std::string> &vec_env)
{

	c.CONTENT_LENGTH = std::to_string(req.message_body.size()); //size of the body
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
	// P("~~~~~~Body size:" << c.CONTENT_LENGTH );
	env_meta_variables(c, vec_env);
}

void init_execve_cgi(t_http_req const& req, std::vector<std::string> &execve_param)
{
	std::string executable = "/usr/bin/php";

	if (req.loc->CGI.PATH_INFO != std::string("None"))
		executable = req.loc->CGI.SCRIPT_NAME;
	execve_param.push_back(executable);
	execve_param.push_back(req.URL);
}

void write_to_upload_file(int &fd_upload_location, t_http_req &req, std::vector<std::string> vec_env)
{
	int pp[2];
	int fd;
	pid_t pid;

	if (pipe(pp) == -1)
	{
		P("Error: pipe failed");
		throw internal_server_error_exc();
	}
	if ((pid = fork()) == -1)
	{
		P("Error: fork failed");
		throw internal_server_error_exc();
	}
	if (!pid)
	{
		close(pp[1]);
		dup2(pp[0], STDIN_FILENO); // TO GET THE REQUEST BODY AS INPUT FOR PHP-CGI
		dup2(fd_upload_location, STDOUT_FILENO); // TO GET THE OUTPUT OF PHP IN DEFAULT FILE

		std::vector<std::string> execve_param;
		init_execve_cgi(req, execve_param);
		char *tab_env[vec_env.size() + 1];
		char *tab_execve[execve_param.size() + 1];
		for (int i = 0; i < vec_env.size(); i++)
			tab_env[i] = (char*)vec_env[i].c_str();
		for (int i = 0; i < execve_param.size() + 1; i++)
			tab_execve[i] = (char*)execve_param[i].c_str();
		tab_env[vec_env.size()] = nullptr;
		tab_execve[execve_param.size()] = nullptr;
		if (execve(tab_execve[0], tab_execve, tab_env) == -1)
		{
			P("Error: execve cgi php");
			P(strerror(errno));
			close(pp[0]);
			exit(1);
		}
	}
	else
	{
		close(pp[0]);
		write(pp[1], req.message_body.c_str(), req.message_body.size());
		close(pp[1]);
		waitpid(pid, 0, 0);
	}
}

// void parse_cgi_post_file(t_http_req &req, std::string const& ouput_file)
// {
// 	std::ifstream	fd(ouput_file);
// 	std::string		line;
// 	std::string		new_request;

// 	if (!fd.is_open())
// 	{
// 		P("Error: parse_cgi_post_file didn't open");
// 		exit(1);
// 	}
// 	while (getline(fd, line))
// 		new_request += line;

// }

std::string get_cgi(t_http_req &req, t_config &conf, t_server &s)
{
	int fd_upload_location;
	std::vector<std::string> vec_env;
	std::string generated_file_path;

	set_meta_variables(req.loc->CGI, req, conf, s, vec_env);
	generated_file_path = req.loc->root + req.loc->file_upload_location;
	//P("UPLOAD:" << generated_file_path);
	if ((fd_upload_location = open(generated_file_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
	{
		P("Error: file upload location error");
		P(strerror(errno));
		throw internal_server_error_exc();
	}
	write_to_upload_file(fd_upload_location, req, vec_env);
//	parse_cgi_post_file(req, generated_file_path);
	close(fd_upload_location);
	return generated_file_path;
}

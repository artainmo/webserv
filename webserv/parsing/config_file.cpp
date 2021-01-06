#include "../main.hpp"

void parse_location(t_config &conf, std::ifstream &fd, std::string &line)
{
	P(line);
	while (getlinecut(fd, line) && line.substr(0,1) != std::string("}"))
	{
		P(line);
	}
	(void)conf;
}


void  parse(t_config &conf, std::ifstream &fd)
{
	std::string line;

	while (std::getline(fd, line) && line.size() > 5 && line.substr(0, 6) != std::string("server"))
			pass;
	//In server block
	while (getlinecut(fd, line) && line.substr(0,1) != std::string("}"))
	{
		if (line.size() > 7 && line.substr(0, 8) == "location")
			parse_location(conf, fd, line);
	}
	(void)conf;
}

t_config *parse_config(std::string path)
{
	t_config *ret = new t_config;
	std::ifstream fd;

	fd.open(path, std::fstream::in);
	parse(*ret, fd);
	return (ret);	
}

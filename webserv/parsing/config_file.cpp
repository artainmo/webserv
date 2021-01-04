#include "../main.hpp"

t_config parse_line(t_config conf, std::string line)
{

	return (conf);
}

t_config config_file(std::string path)
{
	t_config ret;
	std::ifstream fd;
	std::string line;

	fd.open(path, std::fstream::in)
	while (getline(fd), line)
		ret = parse_line(ret, line);
	return (ret);	
}

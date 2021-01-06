#include "../main.hpp"

bool getlinecut(std::ifstream &fd, std::string &line)
{
	if (!std::getline(fd, line))
		return false;
	line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
	return true;
}

bool check_line(std::string line, const std::string &comp)
{
	unsigned int start = 0;
	unsigned int end = comp.size();

	if (line.size() >= start && line.substr(start, end) == comp)
		return true;
	return false;
}


std::string following_content(std::string line, const std::string &after)
{
	if (line.size() == after.size())
		return std::string("None");
	unsigned int start = after.size() + 1;

	line = line.substr(start, line.size());
	line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
	line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
	return line;
}

std::string parse_until(std::string &line, char until, bool all)
{
	unsigned int i;

	i = 0;
	while(i < line.size() && line[i] != until)
		i++;
	if (all == true && i < line.size())
		return line.substr(0, i + 1);
	return line.substr(0, i);
}

std::string parse_between(std::string &line, char cut, char cut2, bool between)
{
	unsigned int i;
	int start = 0;
	int end = 0;
	
	i = 0;
	while(i < line.size() && line[i] != cut)
		i++;
	if (i == line.size())
		return std::string("None");
	start = i;
	while(i < line.size() && line[i] != cut2)
		i++;
	end = i - start;
	if (between == false)
		return (line.substr(start, end + 1));
	else
		return (line.substr(start + 1, end - 1));
}

void show_conf(t_config &conf)
{
	P(std::string("host: ") + conf.host);
	P(std::string("port: ") + conf.port);
	P(std::string("server_name: ") + conf.server_name);
	P(std::string("default_error_page: ") + conf.default_error_page);
	P(std::string("body_size_limit: ") + conf.body_size_limit);
	if (conf.locations.size() != 0)
	{
		int i = 0;

		for (std::list<t_location>::iterator locs = conf.locations.begin(); locs != conf.locations.end(); locs++)
		{
			i++;
			std::cout << std::string("Location: ");
		   	P(i);
			std::cout << "file_extensions: ";
			for (std::list<std::string>::iterator ext = locs->file_extensions.begin(); ext != locs->file_extensions.end(); ext++)
				std::cout << *ext + std::string(" ");
			std::cout << std::endl;
			std::cout << "link_extension: ";
			P(locs->link_extension);
			std::cout << "http_methods: ";
			for (std::list<std::string>::iterator http = locs->http_methods.begin(); http != locs->http_methods.end(); http++)
				std::cout << *http + std::string(" ");
			std::cout << std::endl;
			std::cout << "root: ";
			P(locs->root);
			std::cout << "directory_listing: ";
			P(locs->directory_listing);
			std::cout << "default_file_if_request_directory: ";
			P(locs->default_file_if_request_directory);
			std::cout << "CGI: ";
			P(locs->CGI);
			std::cout << "file_upload_location: ";
			P(locs->file_upload_location);
		}
	}
}


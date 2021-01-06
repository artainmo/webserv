#include "../main.hpp"

bool getlinecut(std::ifstream &fd, std::string &line)
{
	if (!std::getline(fd, line))
		return false;
	line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
	return true;
}

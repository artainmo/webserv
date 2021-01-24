#include "utils.hpp"

bool	is_non_ascii(std::string s) //Returns true if finds one non-ascii char
{
  int i;

  i = 0;
  while(s[i] != 0)
  {
    if (s[i] < 0 || s[i] > 127)
		  return true;
    i++;
  }
  return false;
}


std::string get_file_extension(std::string path)
{
	return path.substr(path.find_last_of('.') + 1);
}

std::string final_file_in_path(std::string path)
{
  if (path.find_last_of("/") == std::string::npos)
    return path;
	return path.substr(path.find_last_of("/") + 1);
}

int is_regular_file(const char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
		{
			std::cout << "Error: stat function" << std::endl;
			exit(1);
		}
    return S_ISREG(path_stat.st_mode);
}

void print_sockets(std::map<int, std::string> &socket_to_answer)
{
  for (std::map<int, std::string>::iterator i = socket_to_answer.begin(); i != socket_to_answer.end(); i++)
  {
      P("--------------------------------------------------------------------------");
      P("fd: " << i->first);
      P("req: " << i->second);
      P("--------------------------------------------------------------------------");

  }
}

bool equal_to(char c, std::string chars)
{
  for (int i = 0; i < chars.size() ; i++)
    if (chars[i] == c)
      return true;
  return false;
}

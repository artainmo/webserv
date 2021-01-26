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

bool is_white_space(char c)
{
	if (c == '\n' || c == ' ' || c == '\r' || c == '\v' || c == '\f')
		return true;
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

void print_sockets(std::map<int, std::string> &requests)
{
  for (std::map<int, std::string>::iterator i = requests.begin(); i != requests.end(); i++)
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

bool file_exists(std::string name)
{
    std::ifstream f;
    int ret;

    f.open(name);
    ret = f.is_open();
    f.close();
    return ret;
}


void change_directory(std::string relative_path)
{
	char *path_name = new char[100];

	if (!(getcwd(path_name, 100)))
	{
		std::cout << "ERROR: getcwd failed" << std::endl;
		exit(1);
	}
	if (chdir((std::string(path_name) + relative_path).c_str()) == -1)
	{
		std::cout << "ERROR: chdir failed" << std::endl;
		exit(1);
	}
	// getcwd(path_name, 100);
	// std::cout << path_name << std::endl;
	delete [] path_name;
}

#include "utils.hpp"

std::string get_file_extension(std::string path)
{
	return path.substr(path.find_last_of('.') + 1);
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

bool files_in_dir(std::string path_dir, std::list<std::string> &files) //Returns true if directory exists and false if directory does not exist
{
	struct dirent *file;
	DIR *directory;

	if ((directory = opendir(path_dir.c_str())) == 0)
		return false;
	while((file = readdir(directory)) != 0)
		files.push_back(std::string(file->d_name));
	closedir(directory);
	return true;
}

std::string final_file_in_path(std::string path)
{
	return path.substr(path.find_last_of("/") + 1);
}

std::string dir_no_file(std::string path)
{
	std::string ret;
	if (path[0] == '/')
	{
		std::cout << "Error: input dir_no_file"<< std::endl;
	}
	if (path.find("/") == std::string::npos)
		return ".";
	return path.substr(0, path.find_last_of("/") + 1);
}

std::string found(std::string directory, std::string file)
{
	std::string ret;

	if (directory != std::string("."))
		ret = directory + file;
	else
		ret = file;
	if (is_regular_file(ret.c_str()) == 0) //If is a directory
		return "file not found";
	return ret;
}

std::string find_file(std::string path_no_extension)
{
	std::string ret;
	std::string directory;
	std::list<std::string> files;
	std::string file_no_ext;

	directory = dir_no_file(path_no_extension);
	if (files_in_dir(directory, files) == false)
		return "file not found";
	file_no_ext = final_file_in_path(path_no_extension);
	for (std::list<std::string>::iterator i = files.begin(); i != files.end(); i++)
		if ((*i).substr(0, file_no_ext.size()) == file_no_ext)
			if ((*i).size() == file_no_ext.size() || (*i)[file_no_ext.size()] == '.')
			{
				if ((*i)[file_no_ext.size()] == '.')
					return found(directory, *i);
				ret = *i; //If no following . it could be a directory or a file
			}
	if (ret.size() != 0)
		return found(directory, ret);
	return "file not found";
}

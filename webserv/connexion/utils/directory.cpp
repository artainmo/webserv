#include <stdio.h>
#include <dirent.h>
#include <list>
#include <unistd.h>
#include <iostream>

std::string get_file_extension(std::string path)
{
	return path.substr(path.find_last_of('.') + 1);
}

std::list<std::string> files_in_dir(std::string path_dir)
{
	std::list<std::string> files;
	struct dirent *file;
	DIR *directory;

	if ((directory = opendir(path_dir.c_str())) == 0)
	{
		std::cout << "Error: path to directory not found" << std::endl;
		std::cout << path_dir << std::endl;
		exit(1);
	}
	while((file = readdir(directory)) != 0)
		files.push_back(std::string(file->d_name));
	closedir(directory);
	return files;
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

std::string find_file(std::string path_no_extension)
{
	std::string directory = dir_no_file(path_no_extension);
	std::list<std::string> files = files_in_dir(directory);
	std::string file_no_ext = final_file_in_path(path_no_extension);

	for (std::list<std::string>::iterator i = files.begin(); i != files.end(); i++)
		if ((*i).substr(0, file_no_ext.size()) == file_no_ext)
			if ((*i).size() == file_no_ext.size() || (*i)[file_no_ext.size()] == '.')
				return directory + *i;
	return "file not found";
}

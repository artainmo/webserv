#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdio.h>
#include <dirent.h>
#include <list>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

std::string find_file(std::string path_no_extension);
std::string dir_no_file(std::string path);
std::string final_file_in_path(std::string path);
std::list<std::string> files_in_dir(std::string path_dir);
std::string get_file_extension(std::string path);

bool	is_non_ascii(std::string s);

#endif

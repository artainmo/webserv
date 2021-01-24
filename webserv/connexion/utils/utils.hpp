#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdio.h>
#include <dirent.h>
#include <list>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

std::string get_file_extension(std::string path);
bool	is_non_ascii(std::string s);

#endif

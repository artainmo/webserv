#ifndef UTILS_HPP
#define UTILS_HPP

#define P(x) std::cout << x << std::endl

#include <stdio.h>
#include <dirent.h>
#include <list>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <map>

std::string get_file_extension(std::string path);
bool	is_non_ascii(std::string s);
void print_sockets(std::map<int, std::string> &socket_to_answer);

#endif

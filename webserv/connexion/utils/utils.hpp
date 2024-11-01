#ifndef UTILS_HPP
#define UTILS_HPP

#define P(x) std::cout << x << std::endl

#include "../parsing/parsing.hpp"
#include <stdio.h>
#include <dirent.h>
#include <list>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <map>

std::string get_file_extension(std::string path);
std::string final_file_in_path(std::string path);
int is_regular_file(const char *path);
bool is_dir(std::string path_dir);
bool	is_ascii(std::string const &s);
bool is_white_space(char c);
bool equal_to(char c, std::string chars);
void print_sockets(std::map<int, std::string> &requests);
bool file_exists(std::string name);
void change_directory(std::string relative_path);
void show_cgi(std::vector<std::string> &vec_env);
void show_cgi(char **tab);
uint16_t ft_htons(uint16_t x);

#endif

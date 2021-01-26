#include "parsing.hpp"

bool files_in_dir(std::string const &path_dir, std::list<std::string> &files) //Returns true if directory exists and false if directory does not exist
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

std::string dir_no_file(std::string const &path)
{
	std::string ret;
	if (path[0] == '/')
	{
		std::cout << "Error: input dir_no_file"<< std::endl;
		exit(1);
	}
	if (path.find("/") == std::string::npos)
		return ".";
	return path.substr(0, path.find_last_of("/") + 1);
}

std::string found(std::string const &directory, std::string const &file)
{
	std::string ret;

	if (directory != std::string("."))
		ret = directory + file;
	else
		ret = file;
	if (is_regular_file(ret.c_str()) == 0) //If is a directory
		return "directory found";
	return ret;
}

std::string find_file(std::string const &path_no_extension)
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

std::string accordance_method_location(std::string const &url, std::string const &method, t_location *loc)
{
	if (url == std::string("file not found") || loc == 0)
		return url;
	// if (method == std::string("HEAD") || method == std::string("GET")) //Method and get should never be disabled
	// 	return url;
	for (std::list<std::string>::iterator met = loc->http_methods.begin(); met != loc->http_methods.end(); met++)
		if ((method == *met || *met == std::string("ALL")))
			return url;
	return "method not found";
}

std::string find_directory(std::string local_url, std::list<std::string> &index)
{
	std::string ret;

	if (local_url.size() != 0)
		local_url += std::string("/");
	if (index.size() != 0)
	{
		for (std::list<std::string>::iterator i = index.begin(); i != index.end(); i++) //If directory found search for given indexes
		{
			if ((ret = find_file(local_url + *i)) != std::string("file not found") && ret != std::string("directory found"))
				return ret;
		}
	}
	return "file not found";
}

std::string find_file_directory(std::string local_root, std::string const &directory, std::list<std::string> &index, std::string method)
{
	std::string ret;
	std::string local_url;

	// P("local_root: "<< local_root);
	// P("directory: "<< directory);
	if (local_root.size() == 0)
		local_root = directory.substr(0, directory.find_first_of("/")); //If no local root keep the same given path
	if (directory.find("/") != std::string::npos) //Based on tester
		local_url = local_root + directory.substr(directory.find_first_of("/"));
	else
		local_url = local_root;
	if (local_url[0] == '/')
		local_url = local_url.substr(1);
	// P("local_url: "<< local_url);
  if (method == std::string("PUT") || method == std::string("POST"))
    return local_url;
	if ((ret = find_file(local_url)) == std::string("directory found"))
	{
		// P("Searching Dir");
		ret = find_directory(local_url, index);
	}
	return ret;
}

void find_in_file_extension_location(std::string &ret, std::string &last, t_location &loc, t_http_req &req, t_config &conf)
{
		if ((ret = find_file_directory(loc.root, req.URL, loc.index, req.method)) != std::string("file not found"))
		{
			for (std::list<std::string>::iterator ext = loc.file_extensions.begin(); ext != loc.file_extensions.end(); ext++)
			{
				if (get_file_extension(ret) == *ext) //If file extension is equal or all file extensions are accepted
				{
					last = ret;
					req.loc = &loc;
				}
			}
		}
}

void find_in_directory_location(std::string &ret, std::string &last, t_location &loc, t_http_req &req, t_config &conf)
{
	if ((ret = find_file_directory(loc.root, req.URL, loc.index, req.method)) != std::string("file not found"))
	{
		// P("Found: "<< ret);
		last = ret;
		req.loc = &loc;
	}
}

void URL_to_local_path(t_http_req &req, t_config &conf)
{
	std::string ret;
	std::string last;

	last = "file not found";
	if (req.URL.size() != 0 && req.URL[req.URL.size() - 1] == '/')
		req.URL = req.URL.substr(0, req.URL.size() - 1);
	if (req.URL.size() != 0 && req.URL[req.URL.size() - 1] == ':')
		req.URL = req.URL.substr(0, req.URL.size() - 1);
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++) //find location based on directory
	{
		// P("URL: " << req.URL);
		// P("DIR: " << loc->directory);
		// P("COMP: " << (req.URL >= loc->directory && loc->directory != std::string("None")));
		if (req.URL >= loc->directory && loc->directory != std::string("None")) //Find in directory location
			find_in_directory_location(ret, last, *loc, req, conf);
	}
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++) //find location based on file extensions
		if (loc->file_extensions.front() != std::string("None"))
			find_in_file_extension_location(ret, last, *loc, req, conf);
	if (req.loc == 0) //Outside of any locations
		last = find_file_directory(conf.root, req.URL, conf.index, req.method);
	req.URL = last;
	req.URL = accordance_method_location(req.URL, req.method, req.loc);
}

#include "parsing.hpp"

void add_location(t_location &my_loc, t_location &new_loc)
{
	my_loc.active = true;
	my_loc.file_extensions = new_loc.file_extensions;
	my_loc.directory = new_loc.directory;
	// my_loc.http_methods.merge(new_loc.http_methods); //Add accepted methods
	my_loc.http_methods = new_loc.http_methods;
	my_loc.max_body = new_loc.max_body;
	// if (new_loc.root.size() != 0) //If next root is non-existant keep ancient root
	my_loc.root = new_loc.root;
	my_loc.index = new_loc.index;
	my_loc.directory_listing = new_loc.directory_listing;
	my_loc.default_file_if_request_directory = new_loc.default_file_if_request_directory;
	my_loc.CGI = new_loc.CGI;

	my_loc.CGI.active = new_loc.CGI.active;
	my_loc.CGI.AUTH_TYPE = new_loc.CGI.AUTH_TYPE;
	my_loc.CGI.CONTENT_LENGTH = new_loc.CGI.CONTENT_LENGTH;
	my_loc.CGI.CONTENT_TYPE = new_loc.CGI.CONTENT_TYPE;
	my_loc.CGI.GATEWAY_INTERFACE = new_loc.CGI.GATEWAY_INTERFACE;
	my_loc.CGI.PATH_INFO = new_loc.CGI.PATH_INFO;
	my_loc.CGI.PATH_TRANSLATED = new_loc.CGI.PATH_TRANSLATED;
	my_loc.CGI.QUERY_STRING = new_loc.CGI.QUERY_STRING;
	my_loc.CGI.REMOTE_ADDR = new_loc.CGI.REMOTE_ADDR;
	my_loc.CGI.REMOTE_INDENT = new_loc.CGI.REMOTE_INDENT;
	my_loc.CGI.REMOTE_USER = new_loc.CGI.REMOTE_USER;
	my_loc.CGI.REQUEST_METHOD = new_loc.CGI.REQUEST_METHOD;
	my_loc.CGI.REQUEST_URI = new_loc.CGI.REQUEST_URI;
	my_loc.CGI.SCRIPT_NAME = new_loc.CGI.SCRIPT_NAME;
	my_loc.CGI.SERVER_NAME = new_loc.CGI.SERVER_NAME;
	my_loc.CGI.SERVER_PORT = new_loc.CGI.SERVER_PORT;
	my_loc.CGI.SERVER_PROTOCOL = new_loc.CGI.SERVER_PROTOCOL;
	my_loc.CGI.SERVER_SOFTWARE = new_loc.CGI.SERVER_SOFTWARE;

	my_loc.file_upload_location = new_loc.file_upload_location;
}


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
		throw internal_server_error_exc();
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

bool accordance_method_location(std::string const &method, std::list<t_location> &loc)
{
	std::list<t_location>::iterator l = loc.begin();
	std::list<t_location>::iterator rem;
	bool found;

	if (loc.size() == 0)
		return true;
	while (l != loc.end())
	{
		found = false;
		for (std::list<std::string>::iterator met = l->http_methods.begin(); met != l->http_methods.end(); met++)
			if ((method == *met || *met == std::string("ALL")))
				found = true;
		if (found == false)
		{
			rem = l;
			l++;
			loc.erase(rem); //Erase candidate if no adequate method found
		}
		else
			l++;
	}
	if (loc.size() == 0)
		return false;
	return true;
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
	if (directory.find("/") != std::string::npos) //Based on tester
		local_url = local_root + directory.substr(directory.find_first_of("/"));
	else
		local_url = local_root;
	if (local_url[0] == '/')
		local_url = local_url.substr(1);
	// P("local_url: "<< local_url);
  if (method == std::string("PUT"))
    return local_url;
	if ((ret = find_file(local_url)) == std::string("directory found"))
	{
		// P("Searching Dir");
		ret = find_directory(local_url, index);
	}
	return ret;
}

void find_in_file_extension_location(std::list<t_location> &my_locations, t_location &loc, t_http_req &req)
{
	// if ((file = find_file_directory(loc.root, req.URL, loc.index, req.method)) != std::string("file not found"))
	// {
		for (std::list<std::string>::iterator ext = loc.file_extensions.begin(); ext != loc.file_extensions.end(); ext++)
		{
			if (get_file_extension(req.URL) == *ext) //If file extension is equal or all file extensions are accepted
			{
				loc.FOUND_URL = req.URL;
				my_locations.push_back(loc);
			}
		}
	// }
}

void find_in_directory_location(std::list<t_location> &my_locations, t_location &loc, t_http_req &req)
{
	std::string file;
	if ((file = find_file_directory(loc.root, req.URL, loc.index, req.method)) != std::string("file not found"))
	{
		// P("Found: "<< file);
		loc.FOUND_URL = file;
		my_locations.push_back(loc);
	}
}

void prefix_location(std::list<t_location> &my_locations, t_http_req &req, t_config &conf)
{
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++) //find location based on directory
	{
		// P("URL: " << req.URL);
		// P("DIR: " << loc.directory);
		// P("COMP: " << (req.URL >= loc.directory && loc.directory != std::string("None")));
		if (req.URL >= loc->directory && loc->directory != std::string("None")) //Find in directory location
			find_in_directory_location(my_locations, *loc, req);
	}
}

void regular_expression_location(std::list<t_location> &my_locations, t_http_req &req, t_config &conf)
{
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++) //find location based on file extensions
		if (loc->file_extensions.front() != std::string("None"))
			find_in_file_extension_location(my_locations, *loc, req);
}

void set_new_url(std::list<t_location> &my_locations, t_http_req &req)
{
	for (std::list<t_location>::iterator loc = my_locations.begin(); loc != my_locations.end(); loc++)
		if ((*loc).FOUND_URL.size() != 0 && (*loc).FOUND_URL != req.URL)
			req.URL = (*loc).FOUND_URL;
}

void URL_to_local_path(t_http_req &req, t_config &conf)
{
	std::list<t_location> my_locations; //MAP takes location and associated URL

	prefix_location(my_locations, req, conf);
	set_new_url(my_locations, req); //For regular expression match use the URL found in prefix_locations
	regular_expression_location(my_locations, req, conf);
	// P("**********************");
	// show_locations(my_locations);
	// P("**********************");
	if (my_locations.size() == 0) //Outside of any locations set to file not found?
		req.URL = "file not found";
	else if (!accordance_method_location(req.method, my_locations)) //If not one location has asked http method mark it
		req.URL = "method not found";
	else //Add the found location and change URL only to a valid URL
	{
		add_location(req.loc, my_locations.back());
		req.URL = my_locations.back().FOUND_URL;
	}
}

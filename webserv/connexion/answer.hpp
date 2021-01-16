#	ifndef ANSWER_HPP
#	define ANSWER_HPP

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/time.h> // gettimeofday

typedef struct	s_answer_headers
{
	std::string accept_charsets;
	std::string accept_language;
	std::string allow;
	std::string authorization;
	std::string content_language;
	std::string content_length;
	std::string content_location;
	std::string content_type;
	std::string date;
	std::string host;
	std::string last_modified;
	std::string location;
	std::string referer;
	std::string retry_after;
	std::string server;
	std::string transfer_encoding;
	std::string user_agent;
	std::string www_athenticate;
	std::string header_line;
	std::string body;
}				t_answer_headers;


#	endif
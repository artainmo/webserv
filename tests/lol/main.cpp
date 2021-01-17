#include <iostream>
#include <string>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct	_time
{
	std::string	D;
	std::string day;
	std::string month;
	std::string year;
	std::string clock;
};

std::string		parse_time(std::string const& t)
{
	struct _time	info;
	std::string		tab[5];
	size_t			first = 0;
	size_t			len = 0;
	size_t			i = 0;
	size_t			j = 0;

	while (i < t.size())
	{
		if (t[i++] == ' ' || i == t.size())
		{
			tab[j++] = std::string(t, first, len);
			first = i;
			len = 0;
		}
		else
			len++;
	}

	info.D = tab[0];
	info.month = tab[1];
	info.day = tab[2];
	info.clock = tab[3];
	info.year = tab[4];

	return std::string(info.D + ", " 
					+ info.day + " "
					+ info.month + " "
					+ info.year + " "
					+ info.clock + " GMT+1");
}

int main(int argc, char **argv)
{
	char		buf[255];
	struct tm	timeinfo;

	struct stat file;
	if (stat("index.html", &file) == -1)
	{
		printf("stat error\n");
		exit(1);
	}
	//timeinfo = time(file.st_mtime);
	//std::cout << strftime(buf, 200, "Now it's %I:%M%p.", timeinfo) << std::endl; 
	printf("Last file access:         %s\n", ctime(&file.st_mtime));
	strptime(ctime(&file.st_mtime), "%a %b %d %T %Y", &timeinfo);
	strftime(buf, 255, "%a, %d %b %Y %T GMT+1", &timeinfo);
	printf("NEW Last file access:         %s\n", buf);
	//file.st_mtime

	//strftime(buf, sizeof(buf), "%d %b %Y %H:%M", &tm);
//	Last-Modified: Tue, 15 Dec 2020 13:59:38 GMT
}
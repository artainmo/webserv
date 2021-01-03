UNDERSTAND SUBJECT
------------------

World Wide Web (www), grouping of all documents on the web accessible through URLs(uniform resource locator) 

Web browser, is a software application that allows access to the world wide web via URLs. The web browser transforms the URL into an IP address using the DNS (domain name system), it sends http requests towards that ip address or the server machine for the requested content.

Webserv project consists of writing an http web server. Meaning the web server is able to take and answer http requests. 
A real webserver thus at least contains HTML files and an http server. But it can also contain a database to provide dynamic content, meaning the html files take variables from the database to produce dynamic content.

http requests are made by a web browser and consist of different parts, they can request content from the webserver or even post content on the webserver, allowing the client to interact with the webserver.
http stands for hypertext transfer protocol, hypertext are documents that include links to other documents.


NEW ALLOWED FUNCTIONS
-----------------

stat - Information about a file
lstat - Information about a symbolic link
fstat - Same as stat but takes fd as argument

mkdir - creates a new directory
opendir - creates a directory stream
readdir - allowd to read the directory stream
closedir - closes the directory stream

gettimeofday - number of seconds and miniseconds since epoch
strptime - transforms time string in numerical structure




GIT COLLABORATION
-----------------
Never work on same files
Work on separate branches:
git branch <branch name>
git merge

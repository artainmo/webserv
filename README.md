# webserv

Coding a webserver in c++ able to handle HTTP/1.1 requests following a specific configuration file.

**Subject**: [Webserv - 42](https://cdn.intra.42.fr/pdf/pdf/68475/en.subject.pdf)

**Members**: :last_quarter_moon_with_face: [Artainmo](https://github.com/artainmo) - [Ymanzi](https://github.com/ymanzi) :first_quarter_moon_with_face:

![myImage](https://media.giphy.com/media/XRB1uf2F9bGOA/giphy.gif)

## Resources

![myImage](https://media.giphy.com/media/VcizxCUIgaKpa/giphy.gif)

### Server setup
* [Build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
* [Server - Client example code](https://gist.github.com/Alexey-N-Chernyshov/4634731)
* [Simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
* [C++ Web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
* [Youtube - Program your own webserv in C](https://www.youtube.com/watch?v=esXw4bdaZkc)
* [Youtube - Socket Programming in C](https://youtu.be/LtXEMwSG5-8)

### HTTP1.1 RFC
* [HTTP/1.1 (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616.html)
* [HTTP/1.1 : Message Syntax and Routing (RFC 7230)](https://www.rfc-editor.org/rfc/rfc7230.html)
* [HTTP/1.1 : Semantics and Content (RFC 7231)](https://www.rfc-editor.org/rfc/rfc7231.html)
* [HTTP/1.1 : Conditional Requests (RFC 7232)](https://www.rfc-editor.org/rfc/rfc7232.html)
* [HTTP/1.1 : Range Requests (RFC 7233)](https://www.rfc-editor.org/rfc/rfc7233.html)
* [HTTP/1.1 : Caching (RFC 7234)](https://www.rfc-editor.org/rfc/rfc7234.html)
* [HTTP/1.1 : Authentication (RFC 7235)](https://www.rfc-editor.org/rfc/rfc7235.html)

### HTTP Header Syntax
* [HTTP Request Methods](https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods)
* [HTTP Status Codes](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status)
* [HTTP Header Break Style](https://stackoverflow.com/questions/5757290/http-header-line-break-style)

### Select / non-blocking fd / Signals
* [Select](https://www.lowtek.com/sockets/select.html)
* [Youtube - Slect](https://www.youtube.com/watch?v=Y6pFtgRdUts)
* [Non-blocking fd](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)
* [Handle Signal](https://stackoverflow.com/questions/108183/how-to-prevent-sigpipes-or-handle-them-properly)

### CGI
* [Env CGI variables](https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI)
* [CGI : Getting Started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)
* [CGI 1.1 Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)

![myImage](https://media.giphy.com/media/xT5LMTff7lN1riPLXi/giphy.gif)

## Notes
### Understand subject
World Wide Web (www), is a grouping of all documents on the web accessible through URLs (uniform resource locator).

A server is a computer connected to the www, it contains an IP address to identify itself, this IP address can be associated with an URL which is a non-random name for easier identification.

Web browser, is a software application that allows access to the world wide web via URLs. The web browser transforms the URL into an IP address using the DNS (domain name system), it sends http requests towards that ip address or the server machine for the requested content.

Webservers serve an application/platform/website on the web. They receive web browser requests and send back responses. They often run on an external server, which is a computer with no visual display, waiting for client-servers to make requests.

Webserv project consists of writing an http web server. Meaning the web server is able to take and answer http requests.<br>
A real webserver thus at least contains HTML files and an http server. But it can also contain a database to provide dynamic content, meaning the html files take variables from the database to produce dynamic content.

http requests are made by a web browser and consist of different parts, they can request content from the webserver or even post content on the webserver, allowing the client to interact with the webserver.
http stands for hypertext transfer protocol, hypertext are documents that include links to other documents.

An http server is able to accept connections with a client, to be able to receive and answer http requests.

A web platform consists of a back-end and front-end.<br>
Back-end acts as a web-server (answer HTTP request) and data-base-manager.<br>
Front-end refers to the pages send by the web-server to the client-server, that the client's web browser will visually display, creating a user interface (UI).<br>
Different programming languages can be used for the back-end and front-end development.

### HTTP
The language used between servers to communicate.<br>
http requests are made by a web browser and follow a specific syntax, they can request content from the webserver or even post content on the webserver, allowing the client to interact with the webserver.<br>
http responses are made by the web server and follow a specific syntax, they can send HTML pages for the web browser to display.<br>
http stands for hypertext transfer protocol, hypertext are documents that include links to other documents.

Structure of a request:<br>
First line: method + request targer (URL) + HTTP version (HTTP/1.1)<br>
Header fields: Optional extra information about request<br>
Separation empty space<br>
Body: Content of the request

Structure of a response:<br>
Status line: HTTP version (HTTP/1.1) + status code + status short description<br>
Header fields: Optional extra information about response<br>
Separation empty space<br>
Body: Content of the response

The different HTTP methods for requests are:<br>
GET: requests representation of specified resource, specified resource is indicated by URL.<br>
HEAD: asks for a response identical to that of a GET request, but without the response body, is used to verify if any erros.<br>
POST: used to submit an entity to the specified resource, often causing a change in state on the server.<br>
PUT: replaces all current representations of the specified resource with the submitted request entity.<br>
DELETE: deletes the specified resource.

The different status codes for responses are:<br>
200: OK, successful request<br>
201: Created, for put and post methods<br>
400: Bad request, invalid syntax<br>
404: Not found, the URL is not recognized<br>
405: Not allowed, the resource does not accept the method<br>
500: Internal server error, situation the server does not know how to handle

Example http request:
```
std::string http_request_example = "GET / HTTP/1.1\r\n"
			"Accept-Charsets: dwdwdq dwqdwqdwqdwq\r\n"
      "Accept-Language: en-us\r\n"
			"Allow: xwdwdw dwqdwqdwq\r\n"
			"Authorization: dwdwqd dwqdwqdwq\r\n"
			"Content-Language: dfeefe efewf\r\n"
			"Content-Length: dwdwdw wdwdqw\r\n"
			"Content-Location: sdwdwqdwdwd\r\n"
			"Content-Type: ddw dwqdwqd\r\n"
			"Date: 09-9w2-1w1\r\n"
      "Host: localhost:8080\r\n"
			"Last-Modified: 424443 3424324\r\n"
			"Location: c;d';e\r\n"
			"Referer: dwwdwqd dwqdqwdwq\r\n"
			"Retry-After: dwdqwdw wdwqdqwdw\r\n"
			"Server: dwdwdwdwdwdwd\r\n"
			"Transfer-Encoding: fdsfsf34343\r\n"
      "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/604.5.6 (KHTML, like Gecko) Version/11.0.3 Safari/604.5.6\r\n"
			"WWW_Authenticate: 2323wwdwdw\r\n"
      "\r\n"
      "Usually GET requests don\'t have a body\r\n"
      "But I don\'t care in this case :)";
```

### New functions allowed in this project
stat - Information about a file<br>
lstat - Information about a symbolic link<br>
fstat - Same as stat but takes fd as argument<br>

mkdir - creates a new directory<br>
opendir - creates a directory stream<br>
readdir - allowd to read the directory stream<br>
closedir - closes the directory stream<br>

gettimeofday - number of seconds and miniseconds since epoch<br>
strptime - transforms time string in numerical structure

### Socket programming
Socket programming allows two sockets (IP address with specific port) to communicate. The server contains the listen socket, and client, the client socket.

*Create Socket*<br>
socket - used to create a socket<br>
setsockopt - helps manipulate the socket for re-use<br>
bind - binds the socket to a specific ip address and port number

*Server Socket*<br>
listen - listen socket waits for the client socket/sockets to connect<br>
accept - extract first connection request in queue and returns newly connected socket

*Client Socket*<br>
connect - client socket demands connection with server socket

*Listen to multiple clients with one thread using select*<br>
select - Allows the webserver to handle multiple clients. It waits until one of fds become active. Also allows for selection between TCP and UDP socket, http server normally uses TCP

*Other*<br>
send - allows sending a message to another socket<br>
recv - used to reveive a message from a socket<br>
inet_addr - Used to convert an IPv4 address string notation into an int notation<br>
getsockname - returns address of socket<br>
fcntl - funcion able to perform a range of different commands on file descriptors, here can only be used to set a fd as non-blocking

POST sets received body as env var, that can be used by the URI that is called with cgi or not to produce an upload file. The upload file should be returned in body.<br>
*Set env var<br>
*Call upload file with cgi (or not)<br>
*Produced upload file should be returned in body

### Parsing body
If Transfer-encoding header is set to chunked, the content will be sent in multiple requests.<br>
If header-lenght is specified, the content will be send in one request and the lenght of it is specified.

### CGI
When sending content to a browser it has to be in HTML.<br>
If file is not written in HTML it can be transformed to HTML using CGI, an interface that enables to execute an external program that transfomrs the non-HTML file to HTML.

### Server host name and port
Port 1024 and above are not privileged and free to use and will not fail at binding. localhost can bind to ports under 1024.<br>
Not all host addresses can be assigned, locally only network interfaces can be used. All available network addresses can be found using `ifconfig -a`, to parse the useful information use : `ifconfig -a | grep "inet "`.

### Steps
-Take file path as argument<br>
-Parse the file<br>
-Socket connection / routing<br>
-Parse client socket messages<br>
--method<br>
--URI / URL<br>
--protocol version (IPv4 | IPv6)<br>
--Header fields<br>
---Request modifiers<br>
---client info and representation<br>
---metadata<br>
--empty line to indicate end of header<br>
--Message body containing payload body<br>
-http answers<br>
--status line protocol version<br>
--success or error code<br>
--textual reason phrase<br>
--header fields<br>
---server information<br>
---resource metadata<br>
---representational metadata<br>
---Empty line to indicate end of the header<br>
---Message body that contains the payload body

RFC<br>
-Message Syntax And Routing<br>
-Semantics And Content<br>
-Conditional Requests<br>
-Range Requests<br>
-Caching<br>
-Authentification

VERIF<br>
-Check leaks, hangs, select, .....

### Links
[How to make a basic Tcp server Video](https://youtu.be/LtXEMwSG5-8)<br>
[How to make a basic http server video](https://youtu.be/mStnzIEprH8)<br>
[HTTP request parsing https](https://codereview.stackexchange.com/questions/188384/http-request-parser-in-c)

[NGINX workings](http://nginx.org/en/docs/beginners_guide.html)

### Git collaboration
Never work on same files<br>
Work on separate branches:<br>
git branch \<branch name\> -> create branch<br>
git checkout \<branch name\> -> go on branch<br>
git push -u origin \<branch name\> -> push on your branch<br>
git merge \<branch name\> -> from main branch, merge a sub-branch<br>
git branch -d \<branch name\> -> delete your branch

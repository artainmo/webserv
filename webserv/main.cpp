#include "main.hpp"

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



int main(int argc, char **argv)
{
	t_config *config;
	t_http_req *req;

	if (argc != 2)
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
	config = parse_config(argv[1]);
	//while (19)
	//{
	//	http_request = server_connexion(config); 
		req = parse_http_request(http_request_example);
	//}
}

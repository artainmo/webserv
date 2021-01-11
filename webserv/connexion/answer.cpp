#include "connexion.hpp"

void answer_http_request(t_server &s, t_http_req &req)
{
  char	buffer[4096];
  char	http_header[4096] = "HTTP/1.1 200 OK\r\n\n";

  FILE *html_data = fopen("../tests/index.html", "r");
  if (html_data == NULL)
  {
  	std::cout << "ERROR: file open failed" << std::endl;
  	exit(1);
  }

  	//read file contents and send them to client
  	// This is a fine example program but rather insecure
  	// a real program would probably limit the client to certain files

  fgets(buffer, 4096, html_data);
  strcat(http_header, buffer);
  if (send(s.socket_to_answer, std::string(http_header).c_str(), strlen(http_header), 0) == -1)
  {
    std::cout << "Error: send failed" << std::endl;
    exit(1);
  }
  fclose(html_data);
  (void)s;
  (void)req;
}

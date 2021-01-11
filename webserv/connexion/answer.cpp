#include "connexion.hpp"

std::string header_line(std::string protocol_version, std::string code, std::string text)
{
  return protocol_version + std::string(" ") + code + std::string(" ") + text + std::string("\r\n");
}

std::string error_page()
{
  std::ifstream fd("default/error.html");
  std::string file("\n");
  std::string line;

  if (!fd.is_open())
  {
    std::cout << "Error: file opening default/error.html" << std::endl;
    exit(1);
  }
  while (getline(fd, line))
    file += line;
  return header_line("HTTP/1.1", "400", "BAD REQUEST") + file;
}

std::string GET(std::string path)
{
  std::ifstream fd(path);
  std::string file;
  std::string line;

  if (!fd.is_open())
    return error_page();
  while (getline(fd, line))
    file += line;
  return header_line("HTTP/1.1", "200", "OK") + std::string("\n") + std::string(file);
}

std::string parse_method(t_server &s, t_http_req &req)
{
  if (req.method == std::string("GET"))
    return GET("../tests/index.html");
  if (req.method == std::string("HEAD"))
    return header_line("HTTP/1.1", "200", "OK") + std::string("\n");
  else if (req.method == std::string("POST"))
    pass;
  else if (req.method == std::string("PUT"))
    pass;
  else if (req.method == std::string("DELETE"))
    pass;
  else if (req.method == std::string("CONNECT"))
    pass;
  else if (req.method == std::string("OPTIONS"))
    pass;
  else if (req.method == std::string("TRACE"))
    pass;
  (void)s;
  return error_page();
}

void answer_http_request(t_server &s, t_http_req &req)
{
  std::string answer;

  answer = parse_method(s, req);
  if (send(s.socket_to_answer, answer.c_str(), answer.size(), 0) == -1)
  {
    std::cout << "Error: send failed" << std::endl;
    exit(1);
  }
}

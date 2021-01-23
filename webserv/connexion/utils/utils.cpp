#include "utils.hpp"

bool	is_non_ascii(std::string s) //Returns true if finds one non-ascii char
{
  int i;

  i = 0;
  while(s[i] != 0)
  {
    if (s[i] < 0 || s[i] > 127)
		  return true;
    i++;
  }
  return false;
}

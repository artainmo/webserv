int find_body(std::string req)
{
	unsigned int counter;
	bool follow;

	counter = 0;
	follow = false;
	if (req.size() == 0)
		return -1; //Returning error in this case does not seem correct, so return not ready, comes from recv fail
	P("body find:|" << req << "|" << "size: " << req.size());
	req = find_start(req);
	for (unsigned int i = 0; i < req.size() ; i++)
	{
		if (req[i] == '\n')
		{
			if (follow == true) //Following \n found meaning empty line found meaning next line is body
			{
				if (req.substr(i).find_first_not_of(" \t\n\v\f\r") == std::string::npos) //Check if body follows
					return counter;
				follow = false;
			}
			counter++;
		}
		if (req[i] == '\n')
			follow = true;
		else if (req[i] != '\r')
			follow = false;
	}
	return -1; //Returns -1 if not found //If not found it means not complete request
}

std::string g_req = ""

int main()
{
	std::cout << find_body(g_req) << std::endl;
}

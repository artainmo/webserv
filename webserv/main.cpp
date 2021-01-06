#include "main.hpp"

int main(int argc, char **argv)
{
	t_config *config;

	if (argc != 2)
		std::cout << "ERROR: Wrong Program Arguments" << std::endl;
	config = parse_config(argv[1]);	
}

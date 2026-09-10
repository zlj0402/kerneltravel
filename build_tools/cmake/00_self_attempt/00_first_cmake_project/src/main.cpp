#include "utils.hpp"
#include <iostream>

int main(int argc, char **argv) 
{
	timeutil::currentTime();
	std::cout << " ";
	if (argc == 0) {
		std::cout << "No params received\n";
	}
	else {
		for (int i = 0; i < argc; ++i) {
			std::cout << argv[0] << ", ";
		}
	}
	
	std::cout << std::endl;
}
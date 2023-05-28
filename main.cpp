#include <iostream>
#include <fstream>
#include "bmp.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Error: No argument!" << std::endl;
		return 0;
	}
	else {
		std::cout << argv[1] << std::endl;
		std::ifstream srcStream;
		srcStream.open(argv[1], std::ios::binary);
		std::string line;
		if (srcStream.is_open()) {
			Bmp srcimg;
			srcimg.read(argv[1]);
			std::cout << srcimg << std::endl;	
			srcStream.close();
		}
	}
	return 0;
}


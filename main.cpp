#include <iostream>
#include <fstream>
#include <cstring>
#include "bmp.h"

void afineTransform(uint32_t width, uint32_t height, uint8_t pixelSize, uint8_t flag, char* data);

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
			
			char newName[100];
			std::cout << "Enter \"name.bmp\"" << std::endl;
			std::cin >> newName;
			std::cout << "You entered: " << newName << std::endl;

			//transformation
		
			if (argc > 2 && argv[3] == 0) {
				afineTransform(srcimg.dibHeader.imageWidth, srcimg.dibHeader.imageHeight, srcimg.dibHeader.bitsPerPixel/8, 0, (char*)srcimg.data.data());
			}

			srcimg.write(newName);	
			srcStream.close();
		}
	}
	return 0;
}

void afineTransform(uint32_t width, uint32_t height, uint8_t pixelSize, uint8_t flag, char* data) {
	char* bufferData = new char[width * height * pixelSize];

	switch (flag) {
		case 0:
			for (int j = 0; j < height; ++j) {
				for (int i = 0; i < width; ++i) {
					for (int d = 0; d < pixelSize; d++) {
						*((bufferData + pixelSize * (i + (height - 1 - j) * width)) + d) = *((data + pixelSize * (i + j * width)) + d);
					}		
				}
			}
			std::memcpy((void*)data, (void*)bufferData, width * height * pixelSize);
			break;
	}

	delete [] bufferData;
}

#include <iostream>

#include <fstream>
#include <cstring>
#include "bmp.h"
#include "matrix.h"

#define MATRIXTEST
//#define REF_X_MAT_INT (Matrix<int> M(3); std::vector<int> V{1,0,0,0,-1,0,0,0,1}; M = V;)

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
			//std::cout << srcimg << std::endl;
			
			char newName[100];
			std::cout << "Enter \"name.bmp\"" << std::endl;
			std::cin >> newName;
			//std::cout << "You entered: " << newName << std::endl;

			//transformation
		
			if (argc > 2) {
				afineTransform(srcimg.dibHeader.imageWidth, srcimg.dibHeader.imageHeight, srcimg.dibHeader.bitsPerPixel/8, std::stoi(argv[2]), (char*)srcimg.data.data());
			}

			srcimg.write(newName);	
			srcStream.close();
		}
	}
	return 0;
}

void afineTransform(uint32_t width, uint32_t height, uint8_t pixelSize, uint8_t flag, char* data) {
	char* bufferData = new char[width * height * pixelSize];

#ifndef MATRIXTEST
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			for (int d = 0; d < pixelSize; d++) {

				*((bufferData + pixelSize * (i + (height - 1 - j) * width)) + d) = *((data + pixelSize * (i + j * width)) + d);
			}		
		}
	}
	std::memcpy((void*)data, (void*)bufferData, width * height * pixelSize);
#endif
#ifdef MATRIXTEST
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			int o_x = i; //original x
			int o_y = j;
			int o_c_x = o_x - width/2; //original coordinate by center image
			int o_c_y = o_y - height/2;
			std::vector<int> v{o_c_x, o_c_y, 0};
			//REF_X_MAT_INT
			Matrix<int> M(3);
			std::vector<int> V;
			int n_x;
			int n_y;
			switch (flag) {
				case 0:
					//std::cout << "input 0\n";
					V = {1,0,0,0,-1,0,0,0,1};
					M = V;
                        		v = M * v;
                        		n_x = v[0] + width/2; //new x
                        		n_y = v[1] + height/2 - 1;
					break;
				case 1:
					//std::cout << "input 1\n";
					V = {-1,0,0,0,1,0,0,0,1};
					M = V;
 		                       	v = M * v;
               			       	n_x = v[0] + width/2 - 1; //new x
                     			n_y = v[1] + height/2;
                                        break;
				case 2:
					//std::cout << "input 2\n";
					V = {-1,0,0,0,-1,0,0,0,1};
					M = V;
                        		v = M * v;
                        		n_x = v[0] + width/2 - 1; //new x
                        		n_y = v[1] + height/2 - 1;
                                        break;
			}
		       	//M = V;
			//v = M * v;
			//int n_x = v[0] + width/2; //new x
			//int n_y = v[1] + height/2 - 1;
			//std::cout << o_y << " " << n_y << std::endl;
			for (int d = 0; d < pixelSize; d++) {
				*((bufferData + pixelSize * (n_x + n_y * width)) + d) = *((data + pixelSize * (o_x + o_y * width)) + d);
			}
		}
	}
	std::memcpy((void*)data, (void*)bufferData, width * height * pixelSize);
#endif
		

	delete [] bufferData;
}

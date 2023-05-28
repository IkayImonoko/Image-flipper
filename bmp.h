#ifndef BMP
#define BMP
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

#pragma pack(push, 1)

struct BmpFileHeader {
	uint16_t signature{0x4D42};
	uint32_t fileSize{0};
	uint16_t reserved1{0};
	uint16_t reserved2{0};
	uint32_t offsetToPixelArrey{0};
};

struct DibHeader {
	uint32_t headerSize{0};
	int32_t imageWidth{0};
	int32_t imageHeight{0};
	uint16_t planes{1};
	uint16_t bitsPerPixel{0};
	uint32_t compression{0};
	uint32_t imageSize{0};
	int32_t xPixelPerMeter{0};
	int32_t yPixelPerMeter{0};
	uint32_t colorsInColorTable{0};
	uint32_t importantColorCount{0};
	uint32_t redChannelBitmask{0x00ff0000};
	uint32_t greenChannelBitmask{0x0000ff00};
	uint32_t blueChannelBitmask{0x000000ff};
	uint32_t alphaChannelBitmask{0xff000000};
	uint32_t colorSpaceType{0x73524742};
	uint32_t unused[16]{0};
};

#pragma pop

struct Bmp {
	BmpFileHeader fileHeader;
	DibHeader dibHeader;
	std::vector<uint8_t> data;

	Bmp() {
	}
	Bmp(const char* fname) {
		read(fname);
	}

	void read(const char* fname) {
		std::ifstream inputFile(fname, std::ios::binary);
		if (inputFile) {
			inputFile.read((char*)&fileHeader, sizeof(fileHeader));
			if (fileHeader.signature != 0x4D42) {
				throw std::runtime_error("Error! Unrecognized file format.");
			}
			inputFile.read((char*)&dibHeader, sizeof(dibHeader));

			if (dibHeader.bitsPerPixel == 32) {
				if(dibHeader.headerSize >= sizeof(DibHeader)) {
					chekColorMasks();
				} 
				else {
					std::cerr << "Warning! The file \"" << fname << "\" does not seem to contain bit mask information\n";
					throw std::runtime_error("Error! Unrecognized file format.");
                		 }
			}
			
			inputFile.seekg(fileHeader.offsetToPixelArrey, inputFile.beg);

			dibHeader.headerSize = sizeof(DibHeader);
			fileHeader.offsetToPixelArrey = sizeof(BmpFileHeader) + sizeof(DibHeader);
			fileHeader.fileSize = fileHeader.offsetToPixelArrey;

			if (dibHeader.imageHeight < 0) {
				throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!"); //TODO
			}

			data.resize(dibHeader.imageWidth * dibHeader.imageHeight * dibHeader.bitsPerPixel / 8);
			
			uint32_t rawRowSize = dibHeader.imageWidth * dibHeader.bitsPerPixel / 8;

			if (rawRowSize % 4 == 0) {
				inputFile.read((char*)data.data(), data.size());
				fileHeader.fileSize += data.size();
			}
			else {
				uint8_t paddingSize = 4 - (rawRowSize % 4);
				std::vector<uint8_t> paddingData{paddingSize};
				for (int j = 0; j < dibHeader.imageHeight; ++j) {
					inputFile.read((char*)data.data() + rawRowSize * j, rawRowSize);
					inputFile.read((char*)paddingData.data(), paddingSize);
				}
				fileHeader.fileSize += data.size() + dibHeader.imageWidth * paddingSize;
			}


		}	
		else {
			throw std::runtime_error("Error! Unable to open the input image file.");
		}
	}

	//Bmp(int32_t width, int32_t height, bool alpha) {
	//}

	//void write(const char* fname) {
	//}

	friend	std::ostream& operator<<(std::ostream& os, Bmp b) {

        os << "[signature] = " << b.fileHeader.signature 
        << "\n[fileSize] = " << b.fileHeader.fileSize 
        << "\n[reserved1] = " << b.fileHeader.reserved1 
        << "\n[reserved2] = " << b.fileHeader.reserved2  
        << "\n[offsetToPixelArrey] = " << b.fileHeader.offsetToPixelArrey 
        << "\n[headerSize] = " << b.dibHeader.headerSize 
        << "\n[imageWidth] = " << b.dibHeader.imageWidth 
        << "\n[imageHeight] = " << b.dibHeader.imageHeight 
        << "\n[planes] = " << b.dibHeader.planes 
        << "\n[bitsPerPixel] = " << b.dibHeader.bitsPerPixel 
        << "\n[compression] = " << b.dibHeader.compression 
        << "\n[imageSize] = " << b.dibHeader.imageSize 
        << "\n[xPixelPerMeter] = " << b.dibHeader.xPixelPerMeter 
        << "\n[yPixelPerMeter] = " << b.dibHeader.yPixelPerMeter 
        << "\n[colorsInColorTable] = " << b.dibHeader.colorsInColorTable 
        << "\n[importantColorCount] = " << b.dibHeader.importantColorCount 
        << "\n[redChannelBitmask] = " << b.dibHeader.redChannelBitmask 
        << "\n[greenChannelBitmask] = " << b.dibHeader.greenChannelBitmask 
        << "\n[blueChannelBitmask] = " << b.dibHeader.blueChannelBitmask 
        << "\n[alphaChannelBitmask] = " << b.dibHeader.alphaChannelBitmask 
        << "\n[colorSpaceType] = " << b.dibHeader.colorSpaceType 
        << "\n[unused[16]] = " << b.dibHeader.unused << std::endl; 
	return os;
	}

	private:

	void chekColorMasks() {
		if (dibHeader.redChannelBitmask != 0x00ff0000 || dibHeader.greenChannelBitmask != 0x0000ff00 || dibHeader.blueChannelBitmask != 0x000000ff || dibHeader.alphaChannelBitmask !=0xff000000) {
throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
		}
		if (dibHeader.colorSpaceType != 0x73524742) {
			throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
		}
	}

};
#endif

#ifndef BMP_H
#define BMP_H
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
class BMP {
public:
	BMP();
	~BMP();
	bool load(std::string file, bool gs);
	unsigned char* pixels;
	int width;
	int height;

private:
	unsigned char* buffer[2];
	BITMAPFILEHEADER* bmpHeader;
	BITMAPINFOHEADER* bmpInfo;
};


#endif//BMP_H
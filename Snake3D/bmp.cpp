#include "bmp.h"

BMP::BMP() {
	buffer[0] = NULL;
	buffer[1] = NULL;
	bmpHeader = NULL;
	bmpInfo = NULL;
	pixels = NULL;
}
BMP::~BMP() {
	if(pixels != NULL) delete[] pixels;
}

bool BMP::load(std::string file, bool gs) {
	std::ifstream bmp;
	bmp.open(file.c_str(), std::ios::binary);
	if (bmp.good()) {
		buffer[0] = new unsigned char[sizeof(BITMAPFILEHEADER)];
		buffer[1] = new unsigned char[sizeof(BITMAPINFOHEADER)];

		bmp.read((char*)buffer[0], sizeof(BITMAPFILEHEADER));
		bmp.read((char*)buffer[1], sizeof(BITMAPINFOHEADER));
		bmpHeader = (BITMAPFILEHEADER*)buffer[0];
		bmpInfo = (BITMAPINFOHEADER*)buffer[1];

		width = bmpInfo->biWidth;
		height = bmpInfo->biHeight;

		if (width * height * 3 <= (int)bmpInfo->biSizeImage + 100 && gs) {
			std::cout << "Bitmapa nie jest w skali szarosci!\n";
			delete[] buffer[0];
			delete[] buffer[1];
			return false;
		}
		pixels = new unsigned char[bmpInfo->biSizeImage];

		bmp.seekg(bmpHeader->bfOffBits);
		bmp.read((char*)pixels, bmpInfo->biSizeImage);


		delete[] buffer[0];
		delete[] buffer[1];
		std::cout << "Zaladowano bitmape " << file << ". Width: " << width << " Height: " << height << std::endl;
	}
	else return false;
	bmp.close();
	return true;
}
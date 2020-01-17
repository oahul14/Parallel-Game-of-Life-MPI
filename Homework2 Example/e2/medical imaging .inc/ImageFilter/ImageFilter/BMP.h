#pragma once
#include "Image.h"
#include "./lib/bitmap_image.hpp"

class BMP :
	public Image
{
public:
	BMP();
	~BMP();

	string GetType();
	void LoadImage(string in_address);
	void SaveImage(string out_address);
};


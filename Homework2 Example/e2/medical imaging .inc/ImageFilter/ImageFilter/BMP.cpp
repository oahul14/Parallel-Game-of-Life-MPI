#include "BMP.h"

BMP::BMP()
{
	this->type_ = "bmp";
}


BMP::~BMP()
{
}


string BMP::GetType()
{
	return this->type_;
}


void BMP::LoadImage(string in_address) 
{
	// check if file is of appropriate extension
	if (!CheckType(in_address, this->type_))
	{
		cerr << "Error - Unsurpported format " << in_address << endl;
		return;
	}

	// load image
	this->in_address_ = in_address;
	bitmap_image Image(this->in_address_);
	
	if (!Image)
	{
		cerr << "Error - Failed to open " << this->in_address_ << endl;
		system("pause");
		return;
	}

	else
	{
		
		this->height_ = Image.height();
		this->width_ = Image.width();

		pixels_.reserve(height_ * width_);

		for (unsigned int y = 0; y < height_; y++)
		{
			for (unsigned int x = 0; x < width_; x++)
			{
				rgb_t colour;
				Image.get_pixel(x, y, colour);

				Pixel P(x, y, int(colour.red), int(colour.green), int(colour.blue));
				pixels_.push_back(P);

			}
		}

		this->loaded_ = true;
	}
		
}


void BMP::SaveImage(string out_address)
{
	this->saved_ = false; // in case save image in interface is called more than once this needs reseting.

	if (!this->loaded_)
	{
		cerr << "Image not loaded. Call LoadImage()." << endl;
		return;
	}

	if (!CheckType(out_address, this->type_))
	{
		cerr << "Error - Unsurpported format " << out_address << endl;
		return;
	}

	// check out_address is valid
	ofstream  test(out_address);
	if (!test)
	{
		cerr << " \n\n Error - Could not open file " << out_address << " for writing! \n\n" << std::endl;
		return;
	}
	test.close();

	this->out_address_ = out_address;

	bitmap_image Image(this->width_, this->height_);

	for (Pixel P : pixels_)
	{
		Image.set_pixel(P.x, P.y, P.GetRed(), P.GetGreen(), P.GetBlue());
	}
	Image.save_image(this->out_address_);
	this->saved_ = true;
}

#include "Image.h"

Image::Image()
{
	type_ = "Not specified";
}


Image::~Image()
{
}

bool Image::CheckType(string address, string type)
{
	cout << address << endl;
	string t = address.substr(address.size() - 3, 3);
	if (t == type)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Image::FlipVertical()
{
	if (this->loaded_)
	{
		for (unsigned int i = 0; i < pixels_.size(); i++)
		{

			pixels_[i].y = height_ - pixels_[i].y - 1;
		}
	}

	else
	{
		cerr << "Image not loaded. Call LoadImage()." << endl;
	}
}


void Image::FlipHorizontal()
{
	if (this->loaded_)
	{
		for (unsigned int i = 0; i < pixels_.size(); i++)
		{

			pixels_[i].x = width_ - pixels_[i].x - 1;
		}
	}

	else
	{
		cerr << "Image not loaded. Call LoadImage()." << endl;
	}
}


void Image::RotateClockwise()
{
	int x_holder;
	if (this->loaded_)
	{
		for (unsigned int i = 0; i < pixels_.size(); i++)
		{
			x_holder = pixels_[i].x;
			pixels_[i].x = width_ - pixels_[i].y - 1 -(abs(int(width_)- int(height_)));
			pixels_[i].y = x_holder;
		}	
		
		// Switch height and width
		int w_holder = this->width_;
		this->width_ = this->height_;
		this->height_ = w_holder;
	}

	else
	{
		cerr << "Image not loaded. Call LoadImage()." << endl;
	}
}


void Image::RotateAntiClockwise()
{
	int x_holder;
	if (this->loaded_)
	{
		for (unsigned int i = 0; i < pixels_.size(); i++)
		{
			x_holder = pixels_[i].x;
			pixels_[i].x = pixels_[i].y;
			pixels_[i].y = width_ - x_holder - 1;
		}

		// Switch height and width
		int w_holder = this->width_;
		this->width_ = this->height_;
		this->height_ = w_holder;
	}

	else
	{
		cerr << "Image not loaded. Call LoadImage()." << endl;
	}
}


void Image::BlackAndWhite()
{
	for (unsigned int i = 0; i < pixels_.size(); i++)
	{
		int avg = (pixels_[i].GetBlue() + pixels_[i].GetRed() + pixels_[i].GetGreen()) / 3;
		pixels_[i].SetRed(avg);
		pixels_[i].SetGreen(avg);
		pixels_[i].SetBlue(avg);
	}
}


void Image::SortPixelsRowMajor()
{
	// Sorts pixel vector of the image by Row Major form
	if (this->loaded_)
	{
		vector<Pixel> copy_pixels;
		copy_pixels.reserve(this->pixels_.size());
		for (unsigned int i = 0; i < this->pixels_.size(); i++)
		{
			copy_pixels.push_back(this->pixels_[i]);
		}

		for (Pixel P : copy_pixels)
		{
			int k = P.x + P.y * width_;
			this->pixels_[k] = P;
		}
	}
}


vector<int> Image::FindNeighbours(int i)
{
	// finds neighbours indices in a 3x3 kernel of pixel i
	// includes itself
	// returns vector of indices in row majorform 
	vector<int> neighbours;

	if (!this->loaded_)
	{
		cerr << "Image not loaded. Call LoadImage()." << endl;
		return neighbours;
	}

	neighbours.reserve(9);
	neighbours.push_back(i - this->width_ - 1);
	neighbours.push_back(i - this->width_);
	neighbours.push_back(i - this->width_ + 1);
	neighbours.push_back(i - 1);
	neighbours.push_back(i);
	neighbours.push_back(i + 1);
	neighbours.push_back(i + this->width_ - 1);
	neighbours.push_back(i + this->width_);
	neighbours.push_back(i + this->width_ + 1);

	return neighbours;

}


vector<double> Image::ChooseKernel(int n, double strength)
{
	// Four 3x3 Convolution kernels, output vector of kernel given in row major order
	// n = 0 edge detection
	// n = 1 sharpen
	// n = 2 box blur
	// n = 3 gaussian blur
	vector <vector<double>> kernels;
	kernels.reserve(4);


	//Identity kernel
	vector <double> id = { 0,0,0,0,1,0,0,0,0 };
  
  // set different kernels to be applied in the convolution
	vector <double> edge_detection = { -1, -1, -1, -1, 7, -1, -1, -1, -1 };
	vector <double> sharpen = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
	vector <double> box_blur = { 1. / 9., 1. / 9., 1. / 9., 1. / 9., 1. / 9., 1. / 9., 1. / 9., 1. / 9., 1. / 9. };
	vector <double> gaussian_blur = { 1. / 16., 2. / 16., 1. / 16., 2. / 16., 4. / 16., 2. / 16., 1. / 16., 2. / 16., 1. / 16.};


	kernels = { edge_detection, sharpen, box_blur, gaussian_blur };

	vector <double> kernel_choice = kernels[n];
  
	// scale kernels 0 and 1 based on user defined strength 
	if (n == 0 || n == 1)
	{
		for (unsigned int i = 0; i < (int) kernel_choice.size(); i++)
		{

				kernel_choice[i] = id[i] + strength * kernel_choice[i];
		}
	}
	
	return kernel_choice;
}


void Image::Convolve(int kernel_no, double strength)
	// Four 3x3 Convolution kernels, output vector of kernel given in row major order
	// n = 0 edge detection
	// n = 1 sharpen
	// n = 2 box blur
	// n = 3 gaussian blur
{

	//for (int i = 0; i < pixels_.size(); i++)
	//{
	//	cout << pixels_[i].GetRed() << " # ";
	//}


	if (!this->loaded_)
	{
		cerr << "Image not loaded. Call LoadImage()." << endl;
		return;
	}

	// Sort pixel vector to row major order -- to match the arrangement of the kernel vector
	this->SortPixelsRowMajor();

	cout << " \n Convolving..." << endl;
	double r_temp;
	double g_temp;
	double b_temp;
	double rgb_ave;
	double norm_sum;

	vector <int> neighbours;
	vector <Pixel> copy_pixels;
	vector <double> kernel;

	// THIS IS WHERE THE KERNEL IS CHOSEN
	kernel = this->ChooseKernel(kernel_no, strength);

	// make copy of original pixels to be used later
	copy_pixels.reserve(pixels_.size());
	for (unsigned int i = 0; i < pixels_.size(); i++)
	{
		copy_pixels.push_back(pixels_[i]);
	}

	int count = 0;
	// For each pixel, apply changes according to specified kernel
	for (unsigned int i = 0; i < pixels_.size(); i++)
	{
		neighbours.clear();
		neighbours = FindNeighbours(i);

		r_temp = 0.;
		g_temp = 0.;
		b_temp = 0.;

		for (unsigned int n = 0; n < kernel.size(); n++)
		{
			if (neighbours[n] >= 0 && neighbours[n] < pixels_.size())
			{
				// FOR IMPROVEMENT: all this dereferencing can make it hard for the compiler to optimise code
				Pixel P = copy_pixels[neighbours[n]];
				r_temp += kernel[n] * double(P.GetRed());
				g_temp += kernel[n] * double(P.GetGreen());
				b_temp += kernel[n] * double(P.GetBlue());

			}
		}
		
		// Update RGB channels of pixels, round them to integer
		pixels_[i].SetRed((int)round(r_temp));
		pixels_[i].SetGreen((int)round(g_temp));
		pixels_[i].SetBlue((int)round(b_temp));

	}

}

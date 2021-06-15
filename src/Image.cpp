#include "Image.h"
#include <opencv2/core/hal/interface.h>

Image::Image()
{
	_kernel = std::shared_ptr<kernel>(nullptr);
}

Image::Image(size_t x, size_t y, size_t z)
{
	_kernel = std::make_shared<kernel>(x, y, z);
}

Image::Image(cv::Mat& input) : Image(input.rows, input.cols, input.channels())
{
	auto xsize = this->getX();
	auto ysize = this->getY();
	if (input.channels() == 3)
	{
		for (size_t x = 0; x < xsize; x++)
		{
			for (size_t y = 0; y < ysize; y++)
			{
				auto pixel = input.at<cv::Vec3b>(x, y);
				this->GetData(x, y, 0) = pixel[2];
				this->GetData(x, y, 1) = pixel[1];
				this->GetData(x, y, 2) = pixel[0];
			}
		}
	}
	else if (input.channels() == 1)
	{
		for (size_t x = 0; x < xsize; x++)
		{
			for (size_t y = 0; y < ysize; y++)
			{
				auto pixel = input.at<uint8_t>(x, y);
				this->GetData(x, y, 0) = pixel;
			}
		}
	}
}

cv::Mat Image::toOutput()
{
	auto xsize = this->getX();
	auto ysize = this->getY();
	if (this->getZ() == 3)
	{
		auto output = cv::Mat(cv::Size(xsize, ysize), CV_8UC3);
		for (size_t x = 0; x < xsize; x++)
		{
			for (size_t y = 0; y < ysize; y++)
			{
				auto& pixel = output.at<cv::Vec3b>(x, y);
				pixel[2] = this->GetData(x, y, 0);
				pixel[1] = this->GetData(x, y, 1);
				pixel[0] = this->GetData(x, y, 2);
			}
		}
		return output;
	}
	else
	{
		auto output = cv::Mat(cv::Size(xsize, ysize), CV_8UC1);
		for (size_t x = 0; x < xsize; x++)
		{
			for (size_t y = 0; y < ysize; y++)
			{
				auto& pixel = output.at<uint8_t>(x, y);
				pixel = this->GetData(x, y, 0);
			}
		}
		return output;
	}
}

Image Image::Clone()
{
	return Image(*this->_kernel);
}

uint8_t& Image::GetData(size_t x, size_t y, size_t z)
{
	return this->_kernel->getData(x, y, z);
}

Image Image::EmptyClone()
{
	if (_kernel == nullptr)
	{
		return Image(0, 0, 0);
	}
	auto k = this->_kernel;
	return Image(k->getX(), k->getY(), k->getZ());
}

size_t Image::getX()
{
	if (_kernel == nullptr)
	{
		return -1;
	}
	return this->_kernel->getX();
}

size_t Image::getY()
{
	if (_kernel == nullptr)
	{
		return -1;
	}
	return this->_kernel->getY();
}

size_t Image::getZ()
{
	if (_kernel == nullptr)
	{
		return -1;
	}
	return this->_kernel->getZ();
}

Image Image::extractChannel(size_t z)
{
	if (this->getZ() <= 1)
	{
		return *this;
	}
	if (this->getZ() < z)
	{
		return *this;
	}
	auto output = Image(this->getX(), this->getY(), 1);
	for (size_t _x = 0; _x < this->getX(); _x++)
	{
		for (size_t _y = 0; _y < this->getY(); _y++)
		{
			output.GetData(_x, _y, 0) = this->GetData(_x, _y, z);
		}
	}
	return output;
}

bool Image::valid()
{
	if (this->_kernel == nullptr)
	{
		return false;
	}

	return this->_kernel->valid();
}

Image::Image(kernel& k)
{
	if (!k.valid())
	{
		printf("invalid kernel image\n");
	}
	_kernel = std::make_shared<kernel>(k);
}

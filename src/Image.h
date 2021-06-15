#pragma once
#include<memory>
#include"Kernel.h"
#include"opencv2/imgproc.hpp"

class Image
{
public:
	Image();
	Image(size_t x, size_t y, size_t z);
	Image(cv::Mat& input);
	cv::Mat toOutput();
	Image Clone();
	uint8_t& GetData(size_t x, size_t y, size_t z);
	Image EmptyClone();
	size_t getX();
	size_t getY();
	size_t getZ();
	Image extractChannel(size_t z);
	bool valid();
private:
	Image(kernel& k);
	std::shared_ptr<kernel> _kernel;
};


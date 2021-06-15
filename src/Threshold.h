#pragma once
#include"PixelFunction.h"
class Threshold : public PixelFunction
{
public:
	Threshold(uint8_t thresh);
private:
	void Function(size_t x, size_t y, size_t z, Image input, Image output) override;
	uint8_t value;
};


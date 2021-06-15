#pragma once
#include"PixelFunction.h"
class DominantColor : public PixelFunction
{
public:
	DominantColor(uint8_t thresh, float factor);
private:
	void Function(size_t x, size_t y, size_t z, Image input, Image output) override;
	uint8_t value;
	float dom;
};


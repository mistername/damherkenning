#include "Threshold.h"

Threshold::Threshold(uint8_t thresh) : PixelFunction()
{
	this->value = thresh;
}

void Threshold::Function(size_t x, size_t y, size_t z, Image input, Image output)
{
	auto val = input.GetData(x, y, z);
	if (val <= this->value)
	{
		val = 0;
	}
	else
	{
		val = 255;
	}
	output.GetData(x, y, z) = val;
}
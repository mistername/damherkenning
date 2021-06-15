#include "DominantColor.h"

DominantColor::DominantColor(uint8_t thresh, float factor)
{
	this->value = thresh;
	this->dom = factor;
}

void DominantColor::Function(size_t x, size_t y, size_t z, Image input, Image output)
{
	auto pixel = input.GetData(x, y, z);
	if (pixel < this->value)
	{
		output.GetData(x, y, z) = 0;
		return;
	}

	if (input.getZ() == 1)
	{
		output.GetData(x, y, z) = input.GetData(x, y, z);
		return;
	}

	auto tval = 0;
	for (size_t i = 0; i < input.getZ(); i++)
	{
		if (i == z)
		{
			continue;
		}

		auto channelValue = input.GetData(x, y, i);

		//if (channelValue * this->dom > pixel)
		//{
		//	tval += UINT8_MAX;
		//}

		tval += channelValue;
	}

	tval /= input.getZ() - 1;
	tval += this->dom;


	if (tval < pixel)
	{
		output.GetData(x, y, z) = 255;
	}
	else
	{
		output.GetData(x, y, z) = 0;
	}
}
#pragma once

#include"Image.h"
class PixelFunction
{
public:
	Image ApplyFunction(Image input);
protected:
	virtual void Function(size_t x, size_t y, size_t z, Image input, Image output) = 0;
	virtual Image createOutput(Image input);
private:
};


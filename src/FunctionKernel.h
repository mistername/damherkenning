#pragma once
#include<array>
#include"Kernel.h"
#include"Image.h"
#include"PixelFunction.h"

class FunctionKernel : public PixelFunction
{
protected:
	template<size_t T1, size_t T2>
	FunctionKernel(std::array<std::array<uint8_t, T1>, T2> data) : _kernel(T2, T1, 1, data.data()->data())
	{
	}

	kernel _kernel;

	virtual void Function(size_t x, size_t y, size_t z, Image input, Image output) = 0;
public:
};


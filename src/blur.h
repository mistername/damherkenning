#pragma once
#include"FunctionKernel.h"
#include"Image.h"

class Blur : public FunctionKernel
{
public:
	template<size_t T1, size_t T2>
	Blur(std::array<std::array<uint8_t, T1>, T2> data) : FunctionKernel(data)
	{

	}
private:
	void Function(size_t x, size_t y, size_t z, Image input, Image output) override;
};
#pragma once
#include"PixelFunction.h"
class Canny : public PixelFunction
{
public:
	Canny(){

    };
private:
	void Function(size_t x, size_t y, size_t z, Image input, Image output) override;
    Image createOutput(Image output) override;
};


#include "PixelFunction.h"

Image PixelFunction::ApplyFunction(Image input)
{
    auto xsize = input.getX();
    auto ysize = input.getY();
    auto zsize = input.getZ();
    if (xsize == 0 || ysize == 0 || zsize == 0)
    {
        return Image();
    }

    auto output = this->createOutput(input);

    for (size_t x = 0; x < xsize; x++)
    {
        for (size_t y = 0; y < ysize; y++)
        {
            for (size_t z = 0; z < zsize; z++)
            {
                this->Function(x, y, z, input, output);
            }
        }
    }

    return output;
}

Image PixelFunction::createOutput(Image input)
{
    return input.EmptyClone();
}

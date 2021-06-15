#include"blur.h"
#include<algorithm>


void Blur::Function(size_t x, size_t y, size_t z, Image input, Image output)
{
	auto kx = this->_kernel.getX();
	auto ky = this->_kernel.getY();
	auto kz = this->_kernel.getZ();

	auto kdx = ((kx - 1) / 2);
	auto kdy = ((ky - 1) / 2);
	auto kdz = ((kz - 1) / 2);

	auto value = 0;
	for (size_t xc = 0; xc < kx; xc++)
	{
		auto kox = xc - kdx;
		for (size_t yc = 0; yc < ky; yc++)
		{
			auto koy = yc - kdy;
			for (size_t zc = 0; zc < kz; zc++)
			{
				auto koz = zc - kdz;
				auto ix = x + kox;
				ix = ix < 0 ? 0 : ix >= input.getX() ? input.getX() - 1 : ix;
				auto iy = y + koy;
				iy = iy < 0 ? 0 : iy >= input.getY() ? input.getY() - 1 : iy;
				auto iz = z + koz;
				iz = iz < 0 ? 0 : iz >= input.getZ() ? input.getZ() - 1 : iz;
				value += input.GetData(ix, iy, iz) * this->_kernel.getData(xc, yc, zc);
			}
		}
	}
	
	int final_value;
	auto sum = this->_kernel.Sum();
	if (sum == 0)
	{
		final_value = value;
	}
	else
	{
		final_value = value / sum;
	}
	output.GetData(x, y, z) = final_value;
}
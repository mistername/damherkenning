#pragma once
#include<vector>
#include<stdexcept>

class kernel {
public:
	kernel()
	{
		data = std::vector<uint8_t>(0);
	}

	kernel(size_t x, size_t y, size_t z)
	{
		if (x <= 0)
		{
			throw std::invalid_argument("x size 0 or less for kernel");
		}
		if (y <= 0)
		{
			throw std::invalid_argument("y size 0 or less for kernel");
		}
		if (z <= 0)
		{
			throw std::invalid_argument("z size 0 or less for kernel");
		}
		this->x_size = x;
		this->y_size = y;
		this->z_size = z;
		data = std::vector<uint8_t>(x * y * z);
	}

	kernel(size_t x, size_t y, size_t z, uint8_t* d) : kernel(x,y,z)
	{
		for (size_t zc = 0; zc < z; zc++)
		{
			for (size_t yc = 0; yc < y; yc++)
			{
				for (size_t xc = 0; xc < x; xc++)
				{
					auto index = getIndex(xc, yc, zc);
					this->getData(xc, yc, zc) = d[index];
				}
			}
		}
	}

	uint8_t& getData(size_t x, size_t y, size_t z);
	size_t getX();
	size_t getY();
	size_t getZ();
	unsigned int Sum();
	bool valid();
private:
	size_t getIndex(size_t x, size_t y, size_t z);
	uint8_t& getData(size_t index);

	std::vector<uint8_t> data;
	size_t x_size = 0;
	size_t y_size = 0;
	size_t z_size = 0;
};
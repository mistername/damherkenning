#include"Kernel.h"

uint8_t& kernel::getData(size_t x, size_t y, size_t z)
{
	return this->getData(getIndex(x,y,z));
}

size_t kernel::getX()
{
	return this->x_size;
}

size_t kernel::getY()
{
	return this->y_size;
}

size_t kernel::getZ()
{
	return this->z_size;
}

unsigned int kernel::Sum()
{
	unsigned int value = 0;
	for (size_t i = 0; i < this->data.size(); i++)
	{
		value += this->data[i];
	}
	return value;
}

bool kernel::valid()
{
	return !this->data.empty();
}

uint8_t& kernel::getData(size_t index)
{
	return this->data.at(index);
}

size_t kernel::getIndex(size_t x, size_t y, size_t z)
{
	auto zaddress = z;
	auto yaddress = y * z_size;
	auto xaddress = x * y_size * z_size;
	auto totaladdress = xaddress + yaddress + zaddress;
	if (totaladdress >= this->data.size())
	{
		throw std::invalid_argument("total size out of bounds");
	}
	return totaladdress;
}

#pragma once
#include"Image.h"
class TileColor
{
public:
	TileColor();
	TileColor(Image image, std::array<std::pair<size_t, size_t>, 4> points);
	std::array<uint8_t, 3> GetColor();
private:
	bool isPixelInside(size_t x, size_t y);
	Image _image;
	std::array<std::pair<size_t, size_t>, 4> _points = {};
	std::array<uint8_t, 3> _average_color;
};


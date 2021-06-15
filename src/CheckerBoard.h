#pragma once
#include"Image.h"
#include"AverageColor.h"
constexpr auto boardSizeX = 8;
constexpr auto boardSizeY = 8;

class CheckerBoard
{
public:
	CheckerBoard(Image& image);
	bool CalculatePoints();
	std::array<uint8_t, 3> getTileColor(size_t x, size_t y);
	std::array<std::array<bool, boardSizeY>, boardSizeX> getChanged(CheckerBoard& other);
private:
	std::array<std::pair<size_t, size_t>, 4>	getSquare(size_t x, size_t y);
	std::pair<size_t, size_t>					getPoint(size_t x, size_t y);
	Image						_image;
	std::pair<size_t, size_t>	blackLeft	= std::pair<size_t, size_t>(SIZE_MAX,SIZE_MAX);
	std::pair<size_t, size_t>	blackRight	= std::pair<size_t, size_t>(SIZE_MAX, SIZE_MAX);
	std::pair<size_t, size_t>	whiteRight	= std::pair<size_t, size_t>(SIZE_MAX, SIZE_MAX);
	std::pair<size_t, size_t>	whiteLeft	= std::pair<size_t, size_t>(SIZE_MAX, SIZE_MAX);
	std::array<std::array<TileColor, boardSizeY>, boardSizeX> averageColor = std::array<std::array<TileColor, boardSizeY>, boardSizeX>();
};
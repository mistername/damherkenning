#include "AverageColor.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

inline int isLeft(std::pair<size_t, size_t> P0, std::pair<size_t, size_t> P1, std::pair<size_t, size_t> P2)
{
	return ((P1.first - P0.first) * (P2.second - P0.second) - (P2.first - P0.first) * (P1.second - P0.second));
}
//===================================================================

// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
int cn_PnPoly(std::pair<size_t, size_t> P, std::array< std::pair<size_t, size_t>,4> V)
{
	int    cn = 0;    // the  crossing number counter

	// loop through all edges of the polygon
	for (int i = 0; i < V.size(); i++) {    // edge from V[i]  to V[i+1]
		auto next = (i + 1) % V.size();
		if (((V[i].second <= P.second) && (V[next].second > P.second))     // an upward crossing
			|| ((V[i].second > P.second) && (V[next].second <= P.second))) { // a downward crossing
				// compute  the actual edge-ray intersect x-coordinate
			float vt = (float)(P.second - V[i].second) / (V[next].second - V[i].second);
			if (P.first < V[i].first + vt * (V[next].first - V[i].first)) // P.x < intersect
				++cn;   // a valid crossing of y=P.y right of P.x
		}
	}
	return (cn & 1);    // 0 if even (out), and 1 if  odd (in)
}

TileColor::TileColor()
{
	_average_color = { 0,0,0 };
}

TileColor::TileColor(Image image, std::array<std::pair<size_t, size_t>, 4> points)
{
	_image = image;
	_points = points;
	_average_color = { 0,0,0 };
}

std::array<uint8_t, 3> TileColor::GetColor()
{
	if (this->_average_color[0] != 0 || this->_average_color[0] != 0 || this->_average_color[0] != 0)
	{
		return this->_average_color;
	}

	if (!this->_image.valid())
	{
		return { 0,0,0 };
	}

	size_t xmin = SIZE_MAX;
	size_t xmax = 0;
	size_t ymin = SIZE_MAX;
	size_t ymax = 0;
	for (auto point : _points)
	{
		if (point.first >= _image.getX() || point.second >= _image.getY())
		{
			return { 0,0,0 };
		}
		xmin = point.first <= xmin ? point.first : xmin;
		xmax = point.first > xmax ? point.first : xmax;
		ymin = point.second <= ymin ? point.second : ymin;
		ymax = point.second > ymax ? point.second : ymax;
	}
	auto tile = _image.Clone();
	auto count = 0;
	auto color = std::array<size_t, 3>();
	for (size_t x = xmin; x < xmax; x++)
	{
		for (size_t y = ymin; y < ymax; y++)
		{
			if (cn_PnPoly(std::pair<size_t, size_t>(x, y), this->_points) == 0)
			{
				continue;
			}
			count += 1;
			for (size_t z = 0; z < _image.getZ(); z++)
			{
				color[z] += _image.GetData(x, y, z);
				tile.GetData(x, y, z) = 0;
			}
		}
	}

	std::vector<std::vector<cv::Point>> pointcv = {};
	std::vector<cv::Point> pointsc = {};
	for (size_t i = 0; i < _points.size(); i++)
	{
		pointsc.push_back(cv::Point(_points[i].first, _points[i].second));
	}
	pointcv.push_back(pointsc);
	cv::Mat mask = cv::Mat(_image.getX(), _image.getY(), CV_8UC1, cv::Scalar(0));
	cv::fillPoly(mask, pointcv, cv::Scalar(255));
	cv::Scalar colorr = cv::mean(_image.toOutput(), mask);
	float value = colorr.val[0];
	this->_average_color[0] = value;	
	value = colorr.val[1];
	this->_average_color[1] = value;
	value = colorr.val[2];
	this->_average_color[2] = value;
	return _average_color;

	//cv::imshow("tilepixels", tile.toOutput());

	for (size_t i = 0; i < _average_color.size(); i++)
	{
		if (count == 0)
		{
			break;
		}
		this->_average_color[i] = static_cast<uint8_t>(color[i] / count);
	}
	return this->_average_color;
}

bool TileColor::isPixelInside(size_t x, size_t y)
{
	size_t xmin = SIZE_MAX;
	size_t xmax = 0;
	size_t ymin = SIZE_MAX;
	size_t ymax = 0;
	for (auto point : _points)
	{
		xmin = point.first <= xmin ? point.first : xmin;
		xmax = point.first > xmax ? point.first : xmax;
		ymin = point.second <= ymin ? point.second : ymin;
		ymax = point.second > ymax ? point.second : ymax;
	}
	if (x > xmax || x < xmin)
	{
		return false;
	}
	if(y > ymax || y < ymin)
	{
		return false;
	}

	return cn_PnPoly(std::pair<size_t, size_t>(x,y), _points) == 1;
}

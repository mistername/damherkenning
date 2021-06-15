#include "CheckerBoard.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<cmath>
#include"Image.h"
#include"blur.h"
#include"Threshold.h"
#include"DominantColor.h"
#include"Constants.h"

using namespace cv;
using namespace std;


const std::array<std::array<uchar, 3>, 3> guass_kernel = { { {1,2,1}, {2,4,2},{1,2,1} } };

CheckerBoard::CheckerBoard(Image& image)
{
    Blur guassian = Blur(guass_kernel);
    auto blurred = guassian.ApplyFunction(image);
    //blurred = guassian.ApplyFunction(blurred);
	_image = blurred;
}

bool CheckerBoard::CalculatePoints()
{
    auto thresh = DominantColor(ConstantsValues::DominationMinimum.getValue(), ConstantsValues::DominationMultiplier.getValue());
    auto threshed = thresh.ApplyFunction(_image);
    cv::imwrite("threshed.png", threshed.toOutput());
    //cv::imshow("threshed", threshed.toOutput());
    //cv::waitKey(100);
    cv::Mat edges;
    Canny(threshed.toOutput(), edges, 100, 200);
    cv::imwrite("edges.png", edges);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cv::findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    std::cout << contours.size() << std::endl;
    if (contours.size() != 4)
    {
        return false;
    }
    auto imageCenterX = threshed.getX() / 2;
    auto imageCenterY = threshed.getY() / 2;
    std::vector<char> contourColor = std::vector<char>();
    std::vector<pair<size_t, size_t>> boardPoints = std::vector<pair<size_t, size_t>>();
    for (size_t i = 0; i < contours.size(); i++)
    {
        auto contour = contours[i];
        auto middlex = 0;
        auto middley = 0;
        auto closestToCenterX = 0;
        auto closestToCenterY = 0;
        auto closestDistance = INT_MAX;
        for (size_t j = 0; j < contour.size(); j++)
        {
            auto difX = contour[j].x - imageCenterX;
            auto difY = contour[j].y - imageCenterY;
            auto distance = difX * difX + difY * difY;
            if (distance < closestDistance)
            {
                closestToCenterX = contour[j].x;
                closestToCenterY = contour[j].y;
                closestDistance = distance;
            }
            middlex += contour[j].x;
            middley += contour[j].y;
        }
        boardPoints.push_back(pair<size_t, size_t>(closestToCenterX, closestToCenterY));
        middlex /= contour.size();
        middley /= contour.size();

        auto pixelr = threshed.GetData(middley, middlex, 0);
        auto pixelg = threshed.GetData(middley, middlex, 1);
        auto pixelb = threshed.GetData(middley, middlex, 2);
        if (pixelr != 0)
        {
            contourColor.push_back('r');
        }
        else if (pixelg != 0)
        {
            contourColor.push_back('g');
        }
        else if (pixelb != 0)
        {
            contourColor.push_back('b');
        }
        else
        {
            contourColor.push_back('e');
        }
    }

    for (size_t i = 0; i < contourColor.size(); i++)
    {
        if (contourColor[i] == 'r')
        {
            blackLeft = boardPoints[i];
            continue;
        }
        else if (contourColor[i] == 'b')
        {
            whiteRight = boardPoints[i];
            continue;
        }
    }

    for (size_t i = 0; i < boardPoints.size(); i++)
    {
        auto point = boardPoints[i];
        if (point == blackLeft || point == whiteRight)
        {
            continue;
        }

        auto redPoint = blackLeft;
        auto bluePoint = whiteRight;
        auto dif1X = point.first - redPoint.first;
        auto dif1Y = point.second - redPoint.second;
        auto distance1 = dif1X * dif1X + dif1Y * dif1Y;
        auto dif2X = point.first - bluePoint.first;
        auto dif2Y = point.second - bluePoint.second;
        auto distance2 = dif2X * dif2X + dif2Y * dif2Y;
        if (distance1 < distance2)
        {
            blackRight = point;
        }
        else
        {
            whiteLeft = point;
        }

        auto filledLast = false;

        for (size_t j = 0; j < boardPoints.size(); j++)
        {
            auto point2 = boardPoints[j];
            if (point2 == blackRight || point2 == blackLeft || point2 == whiteRight || point2 == whiteLeft)
            {
                continue;
            }
            if (distance1 < distance2)
            {
                whiteLeft = point2;
                break;
            }
            else
            {
                blackRight = point2;
                break;
            }
        }
        break;
    }
    
    for (size_t x = 0; x < boardSizeX; x++)
    {
        for (size_t y = 0; y < boardSizeY; y++)
        {
            auto points = getSquare(x, y);
            this->averageColor[x][y] = TileColor(this->_image, points);
        }
    }

	return true;
}

std::array<uint8_t, 3> CheckerBoard::getTileColor(size_t x, size_t y)
{
    if ((x & 1) ^ (y & 1))
    {
        return { 255,255,255 };
    }
    return this->averageColor[x][y].GetColor();
}

std::array<std::array<bool, boardSizeY>, boardSizeX> CheckerBoard::getChanged(CheckerBoard& other)
{
    auto changed = std::array<std::array<bool, boardSizeY>, boardSizeX>();
    for (size_t x = 0; x < boardSizeX; x++)
    {
        for (size_t y = 0; y < boardSizeY; y++)
        {
            if ((x & 1) ^ (y & 1))
            {
                changed[x][y] = false;
                continue;
            }
            auto original = other.getTileColor(x, y);
            auto newtile = this->getTileColor(x, y);

            auto dif = (abs(original[0] - newtile[0]) + abs(original[1] - newtile[1]) + abs(original[2] - newtile[2]))/3;
            changed[x][y] = (dif >= ConstantsValues::TileDif.getValue());
        }
    }

    auto overlay = _image.toOutput();

    for (size_t x = 0; x < boardSizeX; x++)
    {
        for (size_t y = 0; y < boardSizeY; y++)
        {
            vector<Point> boardPointCV = vector<Point>();
            auto points = getSquare(x, y);
            boardPointCV.push_back(Point(points[0].first, points[0].second));
            boardPointCV.push_back(Point(points[1].first, points[1].second));
            boardPointCV.push_back(Point(points[2].first, points[2].second));
            boardPointCV.push_back(Point(points[3].first, points[3].second));
            auto color = this->getTileColor(x, y);
            auto scolor = Scalar(0, changed[x][y] ? 255 : 0, changed[x][y] ? 0 : 255);
            auto ccolor = Scalar(color[0], color[1], color[2]);
            vector<vector<Point>> boardPointCVs = { boardPointCV };
            fillPoly(overlay, boardPointCVs, scolor);
            drawContours(overlay, boardPointCVs, -1, Scalar(255, 255, 255), 2, LINE_AA);
        }
    }
    
    imwrite("overlay.jpg", overlay);
    //waitKey(100);

    return changed;
}

std::array<std::pair<size_t, size_t>, 4> CheckerBoard::getSquare(size_t x, size_t y)
{
    auto points = std::array<std::pair<size_t, size_t>, 4>();
    points[0] = getPoint(x, y);
    points[1] = getPoint(x+1, y);
    points[2] = getPoint(x+1, y+1);
    points[3] = getPoint(x, y+1);
    return points;
}

std::pair<size_t, size_t> CheckerBoard::getPoint(size_t x, size_t y)
{
    float xco = x / ((float)boardSizeX);
    float yco = y / ((float)boardSizeX);
    auto blackX = blackLeft.first * (1.0f - xco) + blackRight.first * xco;
    auto blacky = blackLeft.second * (1.0f - xco) + blackRight.second * xco;
    auto whitex = whiteRight.first * (1.0f - xco) + whiteLeft.first * xco;
    auto whitey = whiteRight.second * (1.0f - xco) + whiteLeft.second * xco;
    auto blackpoint = std::pair<size_t, size_t>(blackX, blacky);
    auto whitepoint = std::pair<size_t, size_t>(whitex, whitey);
    x = blackpoint.first * (1.0f - yco) + whitepoint.first * yco;
    y = blackpoint.second * (1.0f - yco) + whitepoint.second * yco;
    auto point = std::pair<size_t, size_t>(x, y);
    return point;
}

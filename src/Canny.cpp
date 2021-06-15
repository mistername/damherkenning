#include"Canny.h"
#include<cmath>
#include<algorithm>

void Canny::Function(size_t x, size_t y, size_t z, Image input, Image output)
{
    if (z % 2 == 1)
    {
        return;
    }
    auto xval = (input.GetData(x,y,z)-(UINT8_MAX/2))*2;
    auto yval = (input.GetData(x,y,z+1)-(UINT8_MAX/2))*2;
    auto sum = sqrt(xval*xval+yval*yval)/2;

    auto angle = atan2(xval, yval);
    std::pair<size_t,size_t> neighbor1;
    std::pair<size_t,size_t> neighbor2;
    if (angle >= 67.5 || angle < -67.5)
    {
        neighbor1 = std::pair<size_t,size_t>(x,y+1);
        neighbor2 = std::pair<size_t,size_t>(x,y-1);
    }
    else if (angle >= 22.5)
    {
        neighbor1 = std::pair<size_t,size_t>(x+1,y-1);
        neighbor2 = std::pair<size_t,size_t>(x-1,y+1);
    }
    else if (angle >= -22.5)
    {
        neighbor1 = std::pair<size_t,size_t>(x+1,y);
        neighbor2 = std::pair<size_t,size_t>(x-1,y);
    }
    else if (angle >= -67.5)
    {
        neighbor1 = std::pair<size_t,size_t>(x+1,y+1);
        neighbor2 = std::pair<size_t,size_t>(x-1,y-1);
    }

    if (neighbor1.first < 0)
    {
        neighbor1.first = 0;
    }
    else if(neighbor1.first >= input.getX())
    {
        neighbor1.first = input.getX()-1;
    }
    
    if (neighbor1.second < 0)
    {
        neighbor1.second = 0;
    }
    else if(neighbor1.second >= input.getY())
    {
        neighbor1.second = input.getY()-1;
    }
    
    if (neighbor2.first < 0)
    {
        neighbor2.first = 0;
    }
    else if(neighbor2.first >= input.getX())
    {
        neighbor2.first = input.getX()-1;
    }
    
    if (neighbor2.second < 0)
    {
        neighbor2.second = 0;
    }
    else if(neighbor2.second >= input.getY())
    {
        neighbor2.second = input.getY()-1;
    }

    auto xval1 = (input.GetData(neighbor1.first,neighbor1.second,z)-(UINT8_MAX/2))*2;
    auto yval1 = (input.GetData(neighbor1.first,neighbor1.second,z+1)-(UINT8_MAX/2))*2;
    auto sum1 = sqrt(xval1*xval1+yval1*yval1)/2;


    auto xval2 = (input.GetData(neighbor2.first,neighbor2.second,z)-(UINT8_MAX/2))*2;
    auto yval2 = (input.GetData(neighbor2.first,neighbor2.second,z+1)-(UINT8_MAX/2))*2;
    auto sum2 = sqrt(xval2*xval2+yval2*yval2)/2;

    if (sum < sum1 && sum < sum2)
    {
        sum = 0;
    }

    output.GetData(x,y,z/2) = sum;
}

Image Canny::createOutput(Image input)
{
    auto newx = input.getX();
    auto newy = input.getY();
    auto newz = input.getZ() / 2;

    return Image(newx, newy, newz);
}
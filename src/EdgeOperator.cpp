#include"EdgeOperator.h"

void EdgeOperator::Function(size_t x, size_t y, size_t z, Image input, Image output)
{
    auto max = 0;
    for (size_t xk = 0; xk < this->_kernel.getX(); xk++)
    {
        for (size_t yk = 0; yk < this->_kernel.getY(); yk++)
        {
            auto kval = (int8_t)this->_kernel.getData(xk,yk,0);
            if (kval <= 0)
            {
                continue;
            }
            
            max += kval;
        }
    }

    if(max == 0)
    {
        return;
    }
    
    auto valueup = 0;
    auto valueright = 0;
    for (size_t xk = 0; xk < this->_kernel.getX(); xk++)
    {
        for (size_t yk = 0; yk < this->_kernel.getY(); yk++)
        {
            auto kvalup = (int8_t)this->_kernel.getData(xk,yk,0);
            auto kvalright = (int8_t)this->_kernel.getData((this->_kernel.getX()-1)-yk,xk,0);

            auto xoff = (this->_kernel.getX()/2)-1;
            auto yoff = (this->_kernel.getY()/2)-1;

            auto cx = x + xk - xoff;
            if (cx >= input.getX())
            {
                cx = input.getX()-1;
            }
            else if (cx < 0)
            {
                cx = 0;
            }
            auto cy = y + yk - yoff;
            if (cy >= input.getY())
            {
                cy = input.getY()-1;
            }
            else if (cy < 0)
            {
                cy = 0;
            }

            valueup += input.GetData(cx,cy,z) * kvalup;
            valueright += input.GetData(cx,cy,z) * kvalright; 
        }
    }

    output.GetData(x,y,z*2) = valueup/(max*2) +UINT8_MAX/2;
    output.GetData(x,y,z*2+1) = valueright/(max*2) +UINT8_MAX/2;
}

Image EdgeOperator::createOutput(Image input)
{
    auto newx = input.getX();
    auto newy = input.getY();
    auto newz = input.getZ() * 2;

    return Image(newx, newy, newz);
}
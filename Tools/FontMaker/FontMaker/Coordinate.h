#pragma once

struct Coordinate 
{
    Coordinate() : x(0), y(0)
    {
    
    }

    Coordinate(unsigned int x, unsigned int y) : x(x), y(y)
    {
    
    }

    Coordinate(const Coordinate& coordinate) : x(coordinate.x), y(coordinate.y)
    {
     
    }


    bool operator < (const Coordinate &coordinate) const 
    {
        if (x != coordinate.x) 
            return x < coordinate.x;
        if (y != coordinate.y) 
            return y < coordinate.y;
    }

    unsigned int x;
    unsigned int y;
};
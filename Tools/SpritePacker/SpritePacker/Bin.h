#pragma once

#include "SpriteData.h"
#include "Coordinate.h"
#include "Size.h"

const unsigned int BIN_PADDING = 8;


struct Bin 
{
    Bin(const Bin& bin) :
        data(bin.data),
        coordinate(bin.coordinate),
        size(bin.size)
    {
    
    }

    Bin(SpriteData* data, const Coordinate& coordinate, const Size& size) :
        data(data),
        coordinate(coordinate),
        size(size)
    {
    
    }

    bool Intersects(const Bin& bin) const
    {
        if (coordinate.x >= (bin.coordinate.x + bin.size.width + BIN_PADDING))
            return false;

        if (coordinate.y >= (bin.coordinate.y + bin.size.height + BIN_PADDING))
            return false;

        if (bin.coordinate.x - BIN_PADDING >= (coordinate.x + size.width))
            return false;

        if (bin.coordinate.y - BIN_PADDING >= (coordinate.y + size.height))
            return false;

        return true;
    }

    //Member variables
    SpriteData* data;
    Coordinate coordinate;
    Size size;
};
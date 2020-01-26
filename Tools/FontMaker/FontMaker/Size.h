#pragma once

struct Size 
{
    Size(unsigned int width, unsigned int height) : width(width), height(height)
    {
    
    }

    Size(const Size& size) : width(size.width), height(size.height)
    {

    }

    bool operator < (const Size &size) const 
    {
        if (width != size.width) 
            return width < size.width;
        if (height != size.height) 
            return height < size.height;
    }

    unsigned int width;
    unsigned int height;
};
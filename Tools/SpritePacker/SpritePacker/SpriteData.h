#pragma once

#include <CImg.h>
#include <string>

struct SpriteData
{
    SpriteData(std::string filename, cimg_library::CImg<unsigned char> image) :
        filename(filename),
        image(image)
    {

    }

    SpriteData(const SpriteData& spriteData) :
        filename(spriteData.filename),
        image(spriteData.image)
    {

    }

    std::string filename;
    cimg_library::CImg<unsigned char> image;
};
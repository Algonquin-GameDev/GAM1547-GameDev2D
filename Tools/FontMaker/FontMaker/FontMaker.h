#pragma once

#include "SpriteData.h"
#include "Bin.h"
#include <string>
#include <list>
#include <vector>
#include <map>


//Local constants
const unsigned int MAX_IMAGE_SIZE = 4096;
const unsigned int MAX_FONT_SIZE = 300;

struct Rect
{
	Rect(float x, float y, float w, float h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	Rect()
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->w = 0.0f;
		this->h = 0.0f;
	}

	bool operator==(const Rect& rect) const
	{
		return x == rect.x && y == rect.y && w == rect.w && h == rect.h;

	}

	bool operator!=(const Rect& rect) const
	{
		return x != rect.x || y != rect.y || w != rect.w || h != rect.h;

	}

	float x;
	float y;
	float w;
	float h;
};

struct GlyphData
{
	unsigned char width;      // width of the glyph in pixels
	unsigned char height;     // height of the glyph in pixels
	unsigned char advanceX;   // number of pixels to advance on x axis
	char bearingX;            // x offset of top-left corner from x axis
	char bearingY;            // y offset of top-left corner from y axis
	Rect frame;
};

struct FontData
{
	FontData() :
		lineHeight(0),
		baseLine(0),
		size(0)
	{
	}

	std::map<char, GlyphData> glyphData;
	std::string file;
	std::string characterSet;
	unsigned int lineHeight;
	unsigned int baseLine;
	unsigned int size;
};


//The SpritePacker class will generate a Sprite Atlas (.png and .json files) for an images 
//placed in the input directory, the created Sprite Atlas files will be saved to the output directory
class FontMaker
{
public:
    //Creates the SpritePacker object with the input directory of the images to be used 
    //to create the Sprite Atlas and the output directory designates where the Sprite Atlas
    //should be saved to (consists of a single .png image file and a single .json file)
	FontMaker(const std::string& inputDirectory, const std::string& outputDirectory);
    ~FontMaker();

    //This will pack the images from the input directory into 1 SpriteAtlas and 
    //saves the created .png image and .json file to the output directory
    bool Build(const std::string& characterSet);

    //Returns the number of image files found in the inputDirectory
    unsigned int GetNumberOfFontFilesFound();

private:
	//
	int GetInteger();

	//
	bool Rasterize(const std::string& file, unsigned int size, const std::string& characterSet);

    //Member variables
	std::vector<std::string> m_Fonts;
	std::string m_InputDirectory;
    std::string m_OutputDirectory;
};
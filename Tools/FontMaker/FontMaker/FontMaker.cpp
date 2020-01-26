#include "FontMaker.h"
#include <json.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <ft2build.h>       
#include FT_FREETYPE_H


FontMaker::FontMaker(const std::string& aInputDirectory, const std::string& aOutputDirectory) :
	m_InputDirectory(aInputDirectory),
    m_OutputDirectory(aOutputDirectory)
{
    //Initialize the input search path
    std::string input = m_InputDirectory + "*.ttf";

    WIN32_FIND_DATAA findData;
    HANDLE handle = FindFirstFileA(input.c_str(), &findData);
    if (handle != INVALID_HANDLE_VALUE)
    {
        do 
        {
            //Get the filename and the path
            std::string filename = std::string(findData.cFileName);
            std::string path = m_InputDirectory + filename;
			m_Fonts.push_back(filename);
        } 
        while (FindNextFileA(handle, &findData));

        //Close the find handle
        FindClose(handle);
    }

	//Initialize the input search path
	input = aInputDirectory + "*.otf";

	findData;
	handle = FindFirstFileA(input.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			//Get the filename and the path
			std::string filename = std::string(findData.cFileName);
			std::string path = aInputDirectory + filename;
			m_Fonts.push_back(filename);
		} while (FindNextFileA(handle, &findData));

		//Close the find handle
		FindClose(handle);
	}

	//Sort the strings
	std::sort(m_Fonts.begin(), m_Fonts.end(), std::less<std::string>());

    //Were there any image files in the input directory?
    if (GetNumberOfFontFilesFound() > 0)
    {
        //Output how many image files were found
        std::cout << "Font files found:\n\n";

        //Cycle through and output the image's filenames
        for (unsigned int i = 0; i < GetNumberOfFontFilesFound(); i++)
        {
            std::cout << i + 1 << ". " << m_Fonts.at(i) << std::endl;
        }

        //Add a newline after the above output
        std::cout << std::endl;
    }
    else
    {
        //Log an error message
        std::cout << "There were no fonts in the input directory" << std::endl << std::endl;
        system("pause");
    }
}

FontMaker::~FontMaker()
{

}

bool FontMaker::Build(const std::string& aCharacterSet)
{
    //Ensure there are actually fonts in the input vector
    if (GetNumberOfFontFilesFound() == 0)
    {
        //Log an error message
        std::cout << "Unable to build Sprite Font, there are no fonts in the input directory." << std::endl;

        //And then return false, we couldn't pack the Sprite Atlas
        return false;
    }

    //Get the output
    std::cout << "Which font would you like to build? (1-" << GetNumberOfFontFilesFound() << ")?\n";
	int index = 0;
	while (index <= 0 || index > GetNumberOfFontFilesFound())
	{
		index = GetInteger();
		if (index <= 0 || index > GetNumberOfFontFilesFound())
		{
			std::cout << "Invalid input...\n\n";
		}
	}

	//
	index--;

	//
	std::cout << std::endl << m_Fonts[index] << " was selected.\n\nWhat size would you like the font to be (0-" << MAX_FONT_SIZE << ")?\n";
	int size = 0;
	while (size <= 0 || size > MAX_FONT_SIZE)
	{
		size = GetInteger();
		if (size <= 0 || size > MAX_FONT_SIZE)
		{
			std::cout << "Invalid font size...\n\n";
		}
	}

    //Output a message indicating that building has begun
    std::cout << std::endl << "Building Font..." << std::endl;

	//
	Rasterize(m_Fonts[index], size, aCharacterSet);


	system("pause");

    //Return true
    return true;

}

int FontMaker::GetInteger()
{
	int value = 0;
	while (!(std::cin >> value))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return value;
}

bool FontMaker::Rasterize(const std::string& aFile, unsigned int aSize, const std::string& aCharacterSet)
{
	//The FreeType library used to load .ttf and .otf fonts
	FT_Library freeType;

	//Initialize the FreeType lib, returns zero if successful
	if (FT_Init_FreeType(&freeType) != 0)
	{
		//The FreeType lib failed to initialize
		return false;
	}

	//Get the path for the file
	std::string path = m_InputDirectory + aFile;

	//Generate a font face for the desired ttf font
	FT_Face face;
	if (FT_New_Face(freeType, path.c_str(), 0, &face) != 0)
	{
		//The font face wasn't successfully created?
		return false;
	}

	//Set the pixel size for the font
	FT_Set_Pixel_Sizes(face, 0, aSize);

	//Create the FontData object
	FontData fontData;
	fontData.file = aFile;
	fontData.characterSet = aCharacterSet;
	fontData.size = aSize;

	//Local variables used below
	unsigned int cellWidth = 0;
	unsigned int cellHeight = 0;
	int maxBearing = 0;
	int minHang = 0;
	unsigned int lineHeight = 0;
	std::map<char, unsigned char*> glyphBuffer;

	//Cycle through all the characters in the character set
	for (unsigned int i = 0; i < aCharacterSet.size(); i++)
	{
		//Get the character for the current index
		char character = aCharacterSet.at(i);

		//Load the character
		if (FT_Load_Char(face, character, FT_LOAD_RENDER) != 0)
		{
			//Did the font load the character? Log an error if it didn't
			std::cout << "Failed to load character: " << character << std::endl;

			//Continue loading characters
			continue;
		}

		//Get the glyph data for the character
		GlyphData glyphData = fontData.glyphData[character];

		//Calculate the buffer size needed for the glyph
		size_t size = face->glyph->bitmap.rows * face->glyph->bitmap.width;

		//Get the buffer
		unsigned char* buffer = glyphBuffer[character];
		if (buffer == nullptr)
		{
			buffer = new unsigned char[size];
		}

		//Copy the buffer over
		memcpy(buffer, face->glyph->bitmap.buffer, size);

		//Set the buffer
		glyphBuffer[character] = buffer;

		//Set the glyph data properties
		glyphData.width = face->glyph->bitmap.width;
		glyphData.height = face->glyph->bitmap.rows;
		glyphData.advanceX = (unsigned char)(face->glyph->advance.x / 64);
		glyphData.bearingX = face->glyph->bitmap_left;
		glyphData.bearingY = face->glyph->bitmap_top;

		//Calculate max bearing
		if (glyphData.bearingY > maxBearing)
		{
			maxBearing = glyphData.bearingY;
		}

		//Calculate max width
		if (glyphData.width > cellWidth)
		{
			cellWidth = glyphData.width;
		}

		//Calculate the line height
		if (glyphData.height > lineHeight)
		{
			lineHeight = glyphData.height;
		}

		//Calculate gylph hang
		int glyphHang = glyphData.bearingY - glyphData.height;
		if (glyphHang < minHang)
		{
			minHang = glyphHang;
		}

		//Set the glyph data
		fontData.glyphData[character] = glyphData;
	}

	//Create bitmap font
	cellHeight = maxBearing - minHang;

	//Set the baseline and line height
	fontData.baseLine = maxBearing;
	fontData.lineHeight = lineHeight;

	//Local variables used below
	unsigned int numberOfCells = (unsigned int)fontData.characterSet.size();
	unsigned int cellsPerRow = (unsigned int)ceil(sqrt(numberOfCells));
	unsigned int cellsPerColumn = cellsPerRow;

	//Local variables used below
	Rect rect(0.0f, 0.0f, (float)cellWidth, (float)cellHeight);
	int offsetX = 0;
	int offsetY = 0;
	unsigned int rows = 0;
	unsigned int columns = 0;

	//Set the render target size
	unsigned int spacer = 4;
	unsigned int viewWidth = (cellWidth * cellsPerColumn) + (cellsPerColumn + 1) * spacer;
	unsigned int viewHeight = (cellHeight * cellsPerRow) + (cellsPerRow + 1) * spacer;

	//Create the output image
	const unsigned int depth = 1;
	const unsigned int channels = 4; //RGBA
	cimg_library::CImg<unsigned char> outputImage = cimg_library::CImg<unsigned char>(viewWidth, viewHeight, depth, channels, 0xffffff00);

	//Remove the extension from the file
	std::string file = aFile.substr(0, aFile.size() - 4);

	//Allocate the JSON frames object
	Json::Value outputJson;

	//Set the file's name
	outputJson["file"] = file + "_" + std::to_string(aSize);
	outputJson["characterSet"] = aCharacterSet;
	outputJson["size"] = aSize;
	outputJson["baseline"] = fontData.baseLine;
	outputJson["lineHeight"] = fontData.lineHeight;

	Json::Value glyphs;

	//Cycle through all the characters in the character set and render the glyph data to the render target
	for (unsigned int i = 0; i < fontData.characterSet.size(); i++)
	{ 
		//Get the character for the index
		char character = fontData.characterSet.at(i);

		//Get the glyph data for the character
		GlyphData glyphData = fontData.glyphData[character];

		//Set base offsets
		offsetX = (cellWidth * columns) + (spacer * (columns + 1)) + ((cellWidth - glyphData.width) * 0.5f);
		offsetY = (cellHeight * rows) + (spacer * (rows + 1)) + cellHeight - glyphData.height;

		//set the sourcce frame for the glyph
		glyphData.frame.x = offsetX;
		glyphData.frame.y = offsetY;
		glyphData.frame.w = glyphData.width;
		glyphData.frame.h = glyphData.height;

		//Set the filename
		Json::Value glyphInfo;
		glyphInfo["character"] = std::string(1, character);

		//
		glyphInfo["advanceX"] = glyphData.advanceX;
		glyphInfo["bearingX"] = glyphData.bearingX;
		glyphInfo["bearingY"] = glyphData.bearingY;

		//Set the frame data
		Json::Value frame;
		frame["x"] = glyphData.frame.x;
		frame["y"] = glyphData.frame.y;
		frame["w"] = glyphData.frame.w;
		frame["h"] = glyphData.frame.h;
		glyphInfo["frame"] = frame;

		//Append the glyph info to the glyphs array
		glyphs.append(glyphInfo);

		//Set the glyph data
		fontData.glyphData[character] = glyphData;

		//Calculate the rgba data buffer size and then allocate it
		const size_t size = glyphData.width * glyphData.height;
		const int colorSize = 4;

		//
		if (size > 0)
		{
			//Create the image for the character
			cimg_library::CImg<unsigned char> image(glyphData.frame.w, glyphData.frame.h, 1, colorSize, 0xffffffff);

			//Cycle through the glyph buffer data and set the rgba data and convert it to black and white
			for (unsigned int j = 0; j < size; j++)
			{
				unsigned char* buffer = glyphBuffer[character];
				unsigned char value = buffer[j];
				unsigned char color[4];
				color[0] = value > 0 ? 255 : 0;
				color[1] = value > 0 ? 255 : 0;
				color[2] = value > 0 ? 255 : 0;
				color[3] = value;

				int x = j % (int)glyphData.frame.w;
				int y = j / (int)glyphData.frame.w;
				image.draw_point(x, y, color, 1.0f);
			}

			//Draw the image to the output image
			outputImage.draw_image(offsetX, offsetY, image, 1.0f);
		}

		//We can now delete the glyph buffer data
		if (glyphBuffer[character] != nullptr)
		{
			delete[] glyphBuffer[character];
			glyphBuffer[character] = nullptr;
		}

		//Increment the columns (and rows if necessary)
		columns++;
		if (columns == cellsPerColumn)
		{
			rows++;
			columns = 0;
		}
	}

	outputJson["glyphs"] = glyphs;

	//Write the json data to a string
	Json::StyledWriter styledWriter;
	std::string output = styledWriter.write(outputJson);

	//Open the output file stream for saving the JSON data
	std::string outputJsonPath = m_OutputDirectory + file + "_" + std::to_string(aSize) + ".json";
	std::ofstream outputStream;
	outputStream.open(outputJsonPath, std::ofstream::out | std::ofstream::trunc);

	//Check to make sure the output file stream is open
	if (outputStream.is_open() == true)
	{
	    outputStream.write(output.c_str(), output.length());
	    outputStream.close();
	}

	//Save the output image data to a .png image
	std::string outputImagePath = m_OutputDirectory + file + + "_" + std::to_string(aSize) + ".png";
	outputImage.save_png(outputImagePath.c_str());

	//Free the face object
	FT_Done_Face(face);

	//Free the FreeType library
	FT_Done_FreeType(freeType);

	return true;
}

unsigned int FontMaker::GetNumberOfFontFilesFound()
{
    return m_Fonts.size();
}
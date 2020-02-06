#pragma once

#include "SpriteData.h"
#include "Bin.h"
#include <string>
#include <list>
#include <vector>


//Local constants
const unsigned int MAX_IMAGE_SIZE = 4096;


//The SpritePacker class will generate a Sprite Atlas (.png and .json files) for an images 
//placed in the input directory, the created Sprite Atlas files will be saved to the output directory
class SpritePacker
{
public:
    //Creates the SpritePacker object with the input directory of the images to be used 
    //to create the Sprite Atlas and the output directory designates where the Sprite Atlas
    //should be saved to (consists of a single .png image file and a single .json file)
    SpritePacker(const std::string& inputDirectory, const std::string& outputDirectory);
    ~SpritePacker();

    //This will pack the images from the input directory into 1 SpriteAtlas and 
    //saves the created .png image and .json file to the output directory
    bool Pack();

    //Returns the number of image files found in the inputDirectory
    unsigned int GetNumberOfImageFilesFound();

private:
    //Attempts to place all the Bins in the input vector onto the 4096x4096 Sprite Atlas 
    //any bins that don't fit will be placed in the remainder vector
    bool Place(const std::vector<Bin>& input, std::vector<Bin>& remainder);

    //Attempt to place a single Bin onto the 4096x4096 Sprite Atlas 
    bool Place(Bin bin);

    //Checks to see wether a Bin will fit on the 4096x4096 Sprite Atlas 
    bool DoesFit(const Bin& bin) const;

    //This will actually set the Bin in its location on the 4096x4096 Sprite Atlas 
    void Use(const Bin& bin);

    //This sorts the vector that contains the coordinates for all the top left values
    //It sorts top to bottom, then left to right
    void Sort();

    //Sort the bins, first by width, then by height
    void Sort(std::vector<Bin>& bins);

    //Trim all the 'empty' space from the 4096x4096 Sprite Atlas 
    void Trim();

    //Used for the top lefts sorting
    struct TopToBottomLeftToRightSort
    {
        bool operator()(const Coordinate& a, const Coordinate& b) const
        {
            return (a.x * a.x + a.y * a.y) < (b.x * b.x + b.y * b.y);
        }
    };

    //Member variables
    std::vector<SpriteData> m_Input;
    std::vector<Bin> m_Bins;
    std::list<Coordinate> m_TopLefts;
    std::string m_OutputDirectory;
    unsigned int m_AtlasWidth;
    unsigned int m_AtlasHeight;
    bool m_NeedToSort;
};
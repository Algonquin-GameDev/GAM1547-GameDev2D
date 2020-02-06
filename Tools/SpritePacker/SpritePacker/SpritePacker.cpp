#include "SpritePacker.h"
#include <json.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>


SpritePacker::SpritePacker(const std::string& aInputDirectory, const std::string& aOutputDirectory) : 
    m_OutputDirectory(aOutputDirectory),
    m_AtlasWidth(MAX_IMAGE_SIZE),
    m_AtlasHeight(MAX_IMAGE_SIZE),
    m_NeedToSort(false)
{
    //Initialize the topLefts vector
    m_TopLefts.push_back(Coordinate(BIN_PADDING, BIN_PADDING));

    //Initialize the input search path
    std::string input = aInputDirectory + "*.png";

    //Start the looking for any png image files in the input directory
    WIN32_FIND_DATAA findData;
    HANDLE handle = FindFirstFileA(input.c_str(), &findData);
    if (handle != INVALID_HANDLE_VALUE)
    {
        do 
        {
            //Get the filename and the path
            std::string filename = std::string(findData.cFileName);
            std::string path = aInputDirectory + filename;

            //Load the image data
            cimg_library::CImg<unsigned char> image = cimg_library::CImg<unsigned char>(path.c_str());

            //If the image is RGB we need to force it to have the A (alpga) channel
            if (image.spectrum() == 3)
            {
                image.channels(0, 3);                   // Force 4th channel.
                image.get_shared_channel(3).fill(255);  // Fill it with 255.
            }

            //Create the sprite data
            SpriteData spriteData(filename, image);

            //Add the pair data to the input vector
            m_Input.push_back(spriteData);

        } 
        while (FindNextFileA(handle, &findData));

        //Close the find handle
        FindClose(handle);
    }

    //Were there any image files in the input directory?
    if (GetNumberOfImageFilesFound() > 0)
    {
        //Output how many image files were found
        std::cout << GetNumberOfImageFilesFound() << " image files found" << std::endl;

        //Cycle through and output the image's filenames
        for (unsigned int i = 0; i < GetNumberOfImageFilesFound(); i++)
        {
            std::cout << m_Input.at(i).filename << std::endl;
        }

        //Add a newline after the above output
        std::cout << std::endl;
    }
    else
    {
        //Log an error message
        std::cout << "There were no images in the input directory" << std::endl << std::endl;
        system("pause");
    }
}

SpritePacker::~SpritePacker()
{
    m_Input.clear();
    m_Bins.clear();
    m_TopLefts.clear();
}

bool SpritePacker::Pack()
{
    //Ensure there are actually images in the input vector
    if (GetNumberOfImageFilesFound() == 0)
    {
        //Log an error message
        std::cout << "Unable to pack Sprite Atlas, there are no images in the input directory" << std::endl;

        //And then return false, we couldn't pack the Sprite Atlas
        return false;
    }

    //Get the output
    std::cout << "Enter the output filename (Don't include the extension)" << std::endl;
    std::string outputFile;
    std::cin >> outputFile;

    //Output a message indicating that packing has begun
    std::cout << std::endl << "Packing Sprites..." << std::endl;

    //Check to see if the user appended .png to the image
    size_t found = outputFile.find(".png");
    if (found != std::string::npos)
    {
        outputFile.erase(found, 4);
    }

    //Add the image data to the input bins
    std::vector<Bin> inputBins;
    for (unsigned int i = 0; i < GetNumberOfImageFilesFound(); i++)
    {
        inputBins.push_back(Bin(&m_Input.at(i), Coordinate(), Size(m_Input.at(i).image.width(), m_Input.at(i).image.height())));
    }

    //Sort the input
    Sort(inputBins);

    // A place to store content that didnt fit into the canvas array.
    std::vector<Bin> remainder;

    //Try to pack bins.
    bool success = Place(inputBins, remainder);

    //Notify the user that not all the images could fit in the sprite atlas
    if (success == false)
    {
        //Output a message indicating that packing has begun
        std::cout << "There were too many images to fit in a " << MAX_IMAGE_SIZE << " x " << MAX_IMAGE_SIZE << "Sprite Atlas" << std::endl;
        std::cout << "The following " << remainder.size() << " images were not added to the Sprite Atlas : " << std::endl;
          
        //Output the image files that were not included in the Sprite Atlas
        for (unsigned int i = 0; i < remainder.size(); i++)
        {
            std::cout << remainder.at(i).data->filename << std::endl;
        }
    }

    //Trim the empty atlas space away
    Trim();

    //Create the output image
    const unsigned int depth = 1;
    const unsigned int channels = 4; //RGBA
    cimg_library::CImg<unsigned char> outputImage = cimg_library::CImg<unsigned char>(m_AtlasWidth, m_AtlasHeight, depth, channels, 0);

    //Allocate the JSON frames object
    Json::Value frames;

    //Cycle through the bins and set the JSON data and draw the image data to the output image
    for (std::vector<Bin>::iterator itor = m_Bins.begin(); itor != m_Bins.end(); itor++)
    {
        //Get the sprite data
        SpriteData* spriteData = itor->data;

        //Remove the .png from the image's filename
        std::string key = spriteData->filename;
        size_t found = key.find(".png");
        if (found != std::string::npos)
        {
            key.erase(found, 4);
        }

        //Set the filename
        Json::Value frameInfo;
        frameInfo["filename"] = key;

        //Set the frame data
        Json::Value frame;
        frame["x"] = itor->coordinate.x;
        frame["y"] = itor->coordinate.y;
        frame["w"] = itor->size.width;
        frame["h"] = itor->size.height;
        frameInfo["frame"] = frame;

        //Append the frame info to the frames array
        frames.append(frameInfo);

        //Draw the image
        const float opacity = 1.0f;
        outputImage.draw_image(itor->coordinate.x, itor->coordinate.y, spriteData->image, opacity);
    }

    //Set the frames JSON data
    Json::Value jsonData;
    jsonData["frames"] = frames;

    //Write the json data to a string
    Json::StyledWriter styledWriter;
    std::string output = styledWriter.write(jsonData);

    //Open the output file stream for saving the JSON data
    std::string outputJsonPath = m_OutputDirectory + outputFile + ".json";
    std::ofstream outputStream;
    outputStream.open(outputJsonPath, std::ofstream::out | std::ofstream::trunc);

    //Check to make sure the output file stream is open
    if (outputStream.is_open() == true)
    {
        outputStream.write(output.c_str(), output.length());
        outputStream.close();
    }

    //Output that the Sprite atlas was successfully created
    std::cout << std::endl << outputFile << ".json was successfully created" << std::endl;

    //Save the output image data to a .png image
    std::string outputImagePath = m_OutputDirectory + outputFile + ".png";
    outputImage.save_png(outputImagePath.c_str());

    //Output that the Sprite atlas was successfully created
    std::cout << outputFile << ".png was successfully created" << std::endl;
    system("pause");

    //Return true
    return true;

}

unsigned int SpritePacker::GetNumberOfImageFilesFound()
{
    return m_Input.size();
}

bool SpritePacker::Place(const std::vector<Bin>& aInput, std::vector<Bin>& aRemainder)
{
    bool placedAll = true;

    //Cycle through and try to place all the bins in the bin vector
    for (std::vector<Bin>::const_iterator itor = aInput.begin(); itor != aInput.end(); itor++)
    {
        //Try to place the bin to the bins vector
        if (Place(*itor) == false)
        {
            //Set the placeAll flag to false
            placedAll = false;

            //And add the bin to the remainder bin
            aRemainder.push_back(*itor);
        }
    }

    return placedAll;
}

bool SpritePacker::Place(Bin aBin)
{
    //Sort the top lefts vector
    Sort();

    //Cycle through and check to see if the bin fits
    for (std::list<Coordinate>::iterator itor = m_TopLefts.begin(); itor != m_TopLefts.end(); itor++)
    {
        //Set the bin's coordinates (top left)
        aBin.coordinate = *itor;

        //Check to see if the bin fits
        if (DoesFit(aBin) == true)
        {
            //The bin fits, use it
            Use(aBin);

            //Remove the iterator from the left lefts array
            m_TopLefts.erase(itor);
            return true;
        }
    }

    //If we got here, the bin couldn't be placed
    return false;
}

bool SpritePacker::DoesFit(const Bin& aBin) const
{
    //Check to see if the bin fits horizontally
    if ((aBin.coordinate.x + aBin.size.width + BIN_PADDING) > m_AtlasWidth)
    {
        return false;
    }
        
    //Check to see if the bin fits vertically
    if ((aBin.coordinate.y + aBin.size.height + BIN_PADDING) > m_AtlasHeight)
    {
        return false;
    }
        
    //Check to see if the bin itersects with any of the other bins in the vector
    for (std::vector<Bin>::const_iterator itor = m_Bins.begin(); itor != m_Bins.end(); itor++)
    {
        //If the bin intersects any of the bins in the vector, return false
        if (aBin.Intersects(*itor) == true)
        {
            return false;
        }
    }

    //Its fits, return true
    return true;
}

void SpritePacker::Use(const Bin& aBin) 
{
    //Get the content coordinate and size
    const Coordinate& coordinate = aBin.coordinate;
    const Size& size = aBin.size;

    //Add that data to the top left vector
    m_TopLefts.push_front(Coordinate(coordinate.x + size.width + BIN_PADDING, coordinate.y));
    m_TopLefts.push_back(Coordinate(coordinate.x, coordinate.y + size.height + BIN_PADDING));

    //Add the bin to the bins vector
    m_Bins.push_back(aBin);

    //Set the need to sort flag to true
    m_NeedToSort = true;
}

void SpritePacker::Sort()
{
    //If we don't need to sort, return early
    if (m_NeedToSort == false)
    {
        return;
    }

    //Sort the topLefts vector
    m_TopLefts.sort(TopToBottomLeftToRightSort());
    m_NeedToSort = false;
}

void SpritePacker::Sort(std::vector<Bin>& aBins)
{
    for (unsigned int i = 0; i < aBins.size(); i++)
    {
        for (unsigned int j = i + 1; j < aBins.size(); j++)
        {
            if (aBins.at(j).size.width != aBins.at(i).size.width)
            {
                if (aBins.at(j).size.width > aBins.at(i).size.width)
                {
                    iter_swap(aBins.begin() + i, aBins.begin() + j);
                }
            }
            else
            {
                if (aBins.at(j).size.height > aBins.at(i).size.height)
                {
                    iter_swap(aBins.begin() + i, aBins.begin() + j);
                }
            }
        }
    }
}

void SpritePacker::Trim()
{
    //Cycle through and determine the right and bottom bounds
    unsigned int boundsRight = 0;
    unsigned int boundsBottom = 0;
    for (std::vector<Bin>::iterator itor = m_Bins.begin(); itor != m_Bins.end(); itor++)
    {
        boundsRight = std::max(boundsRight, itor->coordinate.x + itor->size.width);
        boundsBottom = std::max(boundsBottom, itor->coordinate.y + itor->size.height);
    }

    //Update the atlas width and height
    m_AtlasWidth = boundsRight + BIN_PADDING;
    m_AtlasHeight = boundsBottom + BIN_PADDING;
}
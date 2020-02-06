#include "SpritePacker.h"
#include "Shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

const std::string INPUT_DIRECTORY = "\\..\\Input\\";
//const std::string OUTPUT_DIRECTORY = "\\..\\Output\\";
const std::string OUTPUT_DIRECTORY = "\\..\\..\\..\\GameDev2D\\Assets\\Images\\";


std::string GetApplicationDirectory();

int main(int argc, char *argv[])
{
    //Initialize the input and output directories
    std::string inputDirectory = GetApplicationDirectory() + INPUT_DIRECTORY;
    std::string outputDirectory = GetApplicationDirectory() + OUTPUT_DIRECTORY;

    //Pack the sprites, supply the input and output directories
    SpritePacker spritePacker(inputDirectory, outputDirectory);

    //If there were actually some image files in the input directory 
    //we can now pack it into a Sprite Atlas
    if (spritePacker.GetNumberOfImageFilesFound() > 0)
    {
        spritePacker.Pack();
    }

    return 1;
}

std::string GetApplicationDirectory()
{
    char currentDir[MAX_PATH];
    currentDir, GetModuleFileNameA(NULL, currentDir, MAX_PATH);
    PathRemoveFileSpecA(currentDir);
    return std::string(currentDir);
}

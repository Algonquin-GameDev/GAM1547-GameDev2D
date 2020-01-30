#include "FontMaker.h"
#include "Shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

const std::string CHARACTER_SET_NUMERIC = "0123456789";
const std::string CHARACTER_SET_ALPHA_LOWER = "abcdefghijklmnopqrstuvwxyz";
const std::string CHARACTER_SET_ALPHA_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string CHARACTER_SET_ALPHA_FULL = CHARACTER_SET_ALPHA_LOWER + CHARACTER_SET_ALPHA_UPPER;
const std::string CHARACTER_SET_ALPHA_NUMERIC = CHARACTER_SET_ALPHA_FULL + CHARACTER_SET_NUMERIC;
const std::string CHARACTER_SET_ALPHA_NUMERIC_EXT = CHARACTER_SET_ALPHA_NUMERIC + " ,.?!'\"_+-=*:;#/\\@$%&^<>{}[]()";
const std::string CHARACTER_SET = CHARACTER_SET_ALPHA_NUMERIC_EXT;

const std::string INPUT_DIRECTORY = "\\..\\Input\\";
//const std::string OUTPUT_DIRECTORY = "\\..\\Output\\";
const std::string OUTPUT_DIRECTORY = "\\..\\..\\..\\GameDev2D\\Assets\\Fonts\\";



std::string GetApplicationDirectory();

int main(int argc, char *argv[])
{
    //Initialize the input and output directories
    std::string inputDirectory = GetApplicationDirectory() + INPUT_DIRECTORY;
    std::string outputDirectory = GetApplicationDirectory() + OUTPUT_DIRECTORY;

    //Pack the sprites, supply the input and output directories
    FontMaker fontMaker(inputDirectory, outputDirectory);

    //If there were actually some image files in the input directory 
    //we can now pack it into a Sprite Atlas
    if (fontMaker.GetNumberOfFontFilesFound() > 0)
    {
		fontMaker.Build(CHARACTER_SET);
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

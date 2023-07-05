//~~~~~~~~~~~~~~~ imports ~~~~~~~~~~~~~~~~~
#include <fstream>
#include <iostream>
#include "include/flags.h"
#include "unzip.h"
#include "img2img2pdf.h"
#include "general.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



int main(int argc, char** argv) {
    // Read flags
    const flags::args args(argc, argv);
    const auto& files = args.positional();
    const auto help = args.get<bool>("help", false);
    const auto h = args.get<bool>("h", false);
    const auto quality = args.get<int>("q", 100);
    const auto g = args.get<bool>("g", false);
    const auto scale = args.get<float>("r", 1.0f);

    uint8_t inputCounter = 0;
    std::string inputFileName;

    // Help message
    if(help || h) {
        std::cout <<
        "cbz2pdf"                                                                       << std::endl <<
        "options:"                                                                      << std::endl <<
        "-h, -help    - Displays this message."                                         << std::endl <<
        "-q n         - Sets image quality of the pdf file to n (1-100, default=100)."  << std::endl <<
        "-g           - Converts the images to greyscale."                              << std::endl <<
        "-r n         - Resize the images by a scale of n (0.1-1, default=1)."          << std::endl <<
                                                                                        std::endl <<
        "Usage: cbz2pdf \"filename\""
        << std::endl;
        return 0;
    }

    // Check if parameters are within bounds
    if(quality > 100 || quality < 1) {
        std::cout << "Unsupported quality selected." << std::endl;
        return 1;
    }
    if(scale > 1 || scale < 0.1) {
        std::cout << "Unsupported scale selected." << std::endl;
        return 1;
    }

    // Check input file
    for (const auto& file : files) {
        if(inputCounter == 0) {
            inputFileName = (std::string)file;
            inputCounter++;
        } else {
            std::cout << "Only one file at a time is supported." << std::endl;
            return 1;
        }
    }

    std::experimental::filesystem::path inputFilePath(inputFileName);
    if (!std::experimental::filesystem::exists(inputFilePath)) {
        std::cout << "Input file does not exist!" << std::endl;
        return 1;
    }


    // Create a temp folder if it doesn't already exist.
    std::experimental::filesystem::path tempPath(tempName);
    if (!std::experimental::filesystem::exists(tempPath)) {
        std::experimental::filesystem::create_directory(tempName);
    }


    // Extract files from the cbz file to files in the temp directory
    int num_files = 0;
    if(unzipArchive(inputFileName, num_files) != 0) return 1;


    // Convert the images and write to the pdf

    if(convertAndWrite(quality, g, num_files, inputFileName, scale) != 0) return 1;

    // Clean temp folder
    cleanTemp();

    return 0;
}
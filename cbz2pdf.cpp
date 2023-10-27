//~~~~~~~~~~~~~~~ imports ~~~~~~~~~~~~~~~~~
#include <fstream>
#include <iostream>
#include "include/flags.h"
#include "unzip.h"
#include "img2img2pdf.h"
#include "general.h"

#include "include/FL/Fl.H"
#include "include/FL/Fl_Double_Window.H"
#include "include/FL/Fl_Button.H"
#include "include/FL/Fl_Check_Browser.H"
#include "include/FL/Fl_Input.H"
#include "include/FL/Fl_Browser.H"
#include "include/FL/Fl_Multi_Browser.H"
#include "include/FL/Fl_File_Chooser.H"
#include "include/FL/fl_ask.H"
#include "gui_functions.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int main_gui(int argc, char **argv) {
    { mainWindow = new Fl_Double_Window(width, height);
        { convert_button = new Fl_Button(0, 345, 300, 30, "Convert");
            convert_button->callback(convertCallback, nullptr);
        }
        { toggleable_options = new Fl_Check_Browser(170, 35, 125, 70);
        }
        { resolution_input = new Fl_Input(120, 35, 40, 35, "Scale (0.1-1):");
        }
        { quality_input = new Fl_Input(120, 70, 40, 35, "Quality (0-100):");
        }
        { input_files = new Fl_Multi_Browser(5, 115, 290, 195);
            input_files->callback(selectCallback, nullptr);
        }
        { open_button = new Fl_Button(0, 0, 150, 30, "Open");
            open_button->callback(openFilesCallback, nullptr);
        }
        { about_button = new Fl_Button(150, 0, 150, 30, "About");
            about_button->callback(aboutCallback, nullptr);
        }
        { up_button = new Fl_Button(0, 310, 100, 30, "Up");
            up_button->callback(upCallback, nullptr);
        }
        { down_button = new Fl_Button(100, 310, 100, 30, "Down");
            down_button->callback(downCallback, nullptr);
        }
        { remove_button = new Fl_Button(200, 310, 100, 30, "Remove");
            remove_button->callback(removeCallback, nullptr);
        }
        mainWindow->size_range(width, height, width, height);
        mainWindow->label("cbz2pdf");
        mainWindow->end();
    }

    quality_input->value("75");
    resolution_input->value("1.0");
    toggleable_options->add("Greyscale");
    toggleable_options->add("Merge");
    mainWindow->show(argc, argv);
    return Fl::run();
}


int main(int argc, char** argv) {
    // Read flags
    const flags::args args(argc, argv);
    const auto& files = args.positional();
    const auto help = args.get<bool>("help", false);
    const auto h = args.get<bool>("h", false);
    const auto quality = args.get<int>("q", 100);
    const auto g = args.get<bool>("g", false);
    const auto c = args.get<bool>("c", false);
    const auto scale = args.get<float>("r", 1.0f);
    const auto printLicense = args.get<bool>("license", false);

    if(!c) {
        main_gui(argc, argv);
        return 0;
    }

    if(printLicense) {
        printLicenseToTerminal();
    }

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

    // Loop through all files
    for (const auto& file : files) {
        inputFileName = (std::string)file;
        inputCounter++;

        // Check file
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


        // Extract files from the cbz file to the temp directory
        int num_files = 0;
        if(unzipArchive(inputFileName, num_files) != 0) return 1;


        // Convert the images and write to the pdf

        if(convertAndWrite(quality, g, num_files, inputFileName, scale) != 0) return 1;

        // Clean temp folder
        cleanTemp();
    }



    return 0;
}
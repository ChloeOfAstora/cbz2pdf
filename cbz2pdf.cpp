#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include "include/flags.h"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"
extern "C" {
#include "include/miniz.c"
}
extern "C" {
#include "include/pdfgen.c"
}
#include "greyscale.h"

void cleanTemp() {
    std::experimental::filesystem::path tempPath("_temp_images");
    if (std::experimental::filesystem::exists(tempPath)) {
        std::experimental::filesystem::remove_all(tempPath);
    }
}

int main(int argc, char** argv) {
    // Read flags
    const flags::args args(argc, argv);
    const auto& files = args.positional();
    const auto help = args.get<bool>("help", false);
    const auto h = args.get<bool>("h", false);
    const auto quality = args.get<int>("q", 100);
    const auto g = args.get<bool>("g", false);

    uint8_t inputCounter = 0;
    std::string inputFileName;


    // Help message
    if(help || h) {
        std::cout << "cbz2pdf" << std::endl << "options:" << std::endl <<
        "-h, -help    - Displays this message." << std::endl <<
        "-q           - Sets image quality of the pdf file (1-100, default=100)." << std::endl <<
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


    // Create a temp folder if it doesn't already exist
    std::experimental::filesystem::path tempPath("_temp_images");
    if (!std::experimental::filesystem::exists(tempPath)) {
        std::experimental::filesystem::create_directory("_temp_images");
    }

    // Extract files from the cbz file to files in the temp directory
    mz_zip_archive zip_archive;
    mz_bool status;

    memset(&zip_archive, 0, sizeof(zip_archive));

    status = mz_zip_reader_init_file(&zip_archive, inputFileName.c_str(), 0);
    if (!status) {
        std::cout << "Failed to open the file! " << inputFileName.c_str() << std::endl;
        cleanTemp();
        return 1;
    }

    int num_files = mz_zip_reader_get_num_files(&zip_archive); // Get the number of files
    for (int i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            std::cout << "Failed to read file stat for file " << i << std::endl;
            mz_zip_reader_end(&zip_archive);
            cleanTemp();
            return 1;
        }

        void* file_data = malloc(static_cast<size_t>(file_stat.m_uncomp_size));
        if (!file_data) {
            std::cout << "Failed to allocate memory for file " << i << std::endl;
            mz_zip_reader_end(&zip_archive);
            cleanTemp();
            return 1;
        }

        if (!mz_zip_reader_extract_to_mem(&zip_archive, i, file_data, static_cast<size_t>(file_stat.m_uncomp_size), 0)) {
            std::cout << "Failed to extract file " << i << std::endl;
            free(file_data);
            mz_zip_reader_end(&zip_archive);
            cleanTemp();
            return 1;
        }

        // Create a file in the temp directory and write the extracted data to it
        std::experimental::filesystem::path tempFilePath = tempPath / file_stat.m_filename;
        std::ofstream outputFile(tempFilePath, std::ios::binary);
        outputFile.write(reinterpret_cast<const char*>(file_data), file_stat.m_uncomp_size);
        outputFile.close();

        free(file_data);
    }
    mz_zip_reader_end(&zip_archive);

    // Convert the images and write to the pdf

    struct pdf_info info = {
            .creator = "cbz2pdf"
    };
    pdf_doc* doc = pdf_create(0, 0, &info);
    pdf_append_page(doc);
    int counter = 0;
    for (const auto& entry : std::experimental::filesystem::directory_iterator(tempPath)) {
        int x,y,n;
        int channels = 0;
        unsigned char *dataptr = nullptr;
        unsigned char *data = stbi_load((tempPath.string() + "/" + entry.path().filename().string()).c_str(), &x, &y, &n, 0);
        if(data == NULL) {
            std::cout << "could not open " + (tempPath / entry.path()).filename().string() << std::endl;
            cleanTemp();
            return 1;
        }
        if(g) {
            if(n == 4) channels = 2;
            else channels = 1;
            unsigned char *gsdata = new unsigned char[x*y*channels];
            convertToGrayscale(data, x, y, n, gsdata);
            dataptr = gsdata;
            stbi_image_free(data);
        } else {
            channels = n;
            dataptr = data;
        }
        auto filenameNew = (tempPath.string() + "/" + entry.path().filename().string().substr(0,entry.path().filename().string().find_last_of(".")) + ".jpg");
        if(stbi_write_jpg(filenameNew.c_str(), x, y, channels, dataptr, quality) != 0){
            remove((tempPath.string() + "/" + entry.path().filename().string()).c_str());

            pdf_page_set_size(doc, NULL, x, y);
            pdf_add_image_file(doc, NULL, 0, 0, x, y, filenameNew.c_str());
            counter++;
            if(counter < num_files) pdf_append_page(doc);
        }
        stbi_image_free(dataptr);
    }

    pdf_save(doc, (inputFileName.substr(0, inputFileName.find_last_of(".")) + ".pdf").c_str());
    pdf_destroy(doc);

    // Clean temp folder
    cleanTemp();

    return 0;
}
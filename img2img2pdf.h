#ifndef CBZ2PDF_IMG2IMG2PDF_H
#define CBZ2PDF_IMG2IMG2PDF_H
#include "general.h"
#include <experimental/filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "include/stb_image_resize.h"
extern "C" {
#include "include/pdfgen.c"
}
#include "greyscale.h"


int convertAndWrite(int quality, bool g, int num_files, std::string inputFileName, float scale) {

    struct pdf_info info = {
            .creator = "cbz2pdf"
    };
    pdf_doc *doc = pdf_create(0, 0, &info);
    pdf_append_page(doc);
    int counter = 0;
    for (const auto &entry: std::experimental::filesystem::directory_iterator(tempPath)) {
        int x, y, n;
        int channels = 0;
        unsigned char *dataptr = nullptr;
        unsigned char *data = stbi_load((tempPath.string() + "/" + entry.path().filename().string()).c_str(), &x, &y,
                                        &n, 0);
        if (data == NULL) {
            std::cout << "could not open " + (tempPath / entry.path()).filename().string() << std::endl;
            cleanTemp();
            return 1;
        }
        if(scale != 1) {
            int nx = x * scale;
            int ny = y * scale;
            unsigned char *scaledData = new unsigned char[nx * ny * n];
            if(stbir_resize_uint8(data, x, y, 0, scaledData, nx, ny, 0, n) == 0) {
                std::cout << "Unable to resize one or more images, exiting.." << std::endl;
                cleanTemp();
                return 1;
            }
            stbi_image_free(data);
            data = scaledData;
            x = nx;
            y = ny;
        }
        if (g) {
            if (n == 4) channels = 2;
            else channels = 1;
            unsigned char *gsdata = new unsigned char[x * y * channels];
            convertToGrayscale(data, x, y, n, gsdata);
            dataptr = gsdata;
            stbi_image_free(data);
        } else {
            channels = n;
            dataptr = data;
        }
        auto filenameNew = (tempPath.string() + "/" + entry.path().filename().string().substr(0,
        entry.path().filename().string().find_last_of(".")) + ".jpg"); // Horrible, horrible string handeling but whatever
        if (stbi_write_jpg(filenameNew.c_str(), x, y, channels, dataptr, quality) != 0) {
            remove((tempPath.string() + "/" + entry.path().filename().string()).c_str());

            pdf_page_set_size(doc, NULL, x, y);
            pdf_add_image_file(doc, NULL, 0, 0, x, y, filenameNew.c_str());
            counter++;
            if (counter < num_files) pdf_append_page(doc);
        }
        stbi_image_free(dataptr);
    }

    pdf_save(doc, (inputFileName.substr(0, inputFileName.find_last_of(".")) + ".pdf").c_str());
    pdf_destroy(doc);

    return 0;
}
#endif //CBZ2PDF_IMG2IMG2PDF_H

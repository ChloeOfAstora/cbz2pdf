#ifndef CBZ2PDF_GENERAL_H
#define CBZ2PDF_GENERAL_H
#include <experimental/filesystem>
// I know global variables are "bad" but this one is useful
const auto tempName = "_temp_images_cbz2pdf"; // So I don't have to hard-code paths for it
std::experimental::filesystem::path tempPath(tempName);

void cleanTemp() {
    if (std::experimental::filesystem::exists(tempPath)) {
        std::experimental::filesystem::remove_all(tempPath);
    }
}

void printLicenseToTerminal() {
    std::cout << "\n" <<
             "Copyright 2023 ChloeOfAstora" << "\n" <<
             "" << "\n" <<
             "Permission is hereby granted, free of charge, to any person" << "\n" <<
             "obtaining a copy of this software and associated documentation" << "\n" <<
             "files (the “Software”), to deal in the Software without restriction," << "\n" <<
             "including without limitation the rights to use, copy, modify, merge," << "\n" <<
             "publish, distribute, sublicense, and/or sell copies of the Software," << "\n" <<
             "and to permit persons to whom the Software is furnished to do so," << "\n" <<
             "subject to the following conditions:" << "\n" <<
             "" << "\n" <<
             "The above copyright notice and this permission notice shall be included" << "\n" <<
             "in all copies or substantial portions of the Software." << "\n" <<
             "" << "\n" <<
             "THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND," << "\n" <<
             "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF" << "\n" <<
             "MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT." << "\n" <<
             "IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY" << "\n" <<
             "CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT," << "\n" <<
             "TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE" << "\n" <<
             "SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE." << "\n" <<
             "" << "\n" <<
             "" << "\n" <<
             "3rd party software used:" << "\n" <<
             "" << "\n" <<
             "PDFGen" << "\n" <<
             "Miniz" << "\n" <<
             "Flags" << "\n" <<
             "stb_image.h" << "\n" <<
             "stb_image_write.h" << "\n" <<
             "stb_image_resize.h" << "\n" <<
             "FLTK" << "\n" <<
             "" << "\n" <<
             "" << "\n" <<
             "" << "\n" <<
             "License for miniz:"<< "\n" <<
             " Copyright 2013-2014 RAD Game Tools and Valve Software\n Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC\n \n All Rights Reserved.\n \n Permission is hereby granted, free of charge, to any person obtaining a copy\n of this software and associated documentation files (the \"Software\"), to deal\n in the Software without restriction, including without limitation the rights\n to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n copies of the Software, and to permit persons to whom the Software is\n furnished to do so, subject to the following conditions:\n \n The above copyright notice and this permission notice shall be included in\n all copies or substantial portions of the Software.\n \n THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n THE SOFTWARE.\n \n";
}

#endif //CBZ2PDF_GENERAL_H

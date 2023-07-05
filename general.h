#ifndef CBZ2PDF_GENERAL_H
#define CBZ2PDF_GENERAL_H
#include <experimental/filesystem>
// I know global variables are "bad" but this one is useful
const auto tempName = "_temp_images"; // So I don't have to hard-code paths for it
std::experimental::filesystem::path tempPath(tempName);

void cleanTemp() {
    if (std::experimental::filesystem::exists(tempPath)) {
        std::experimental::filesystem::remove_all(tempPath);
    }
}

#endif //CBZ2PDF_GENERAL_H

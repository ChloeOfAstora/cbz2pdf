#ifndef CBZ2PDF_UNZIP_H
#define CBZ2PDF_UNZIP_H
extern "C" {
#include "include/miniz.c"
}
#include "string"
#include "general.h"

int unzipArchive(std::string inputFileName, int &num_files) {
    num_files = 0;
    mz_zip_archive zip_archive;
    mz_bool status;

    memset(&zip_archive, 0, sizeof(zip_archive));

    status = mz_zip_reader_init_file(&zip_archive, inputFileName.c_str(), 0);
    if (!status) {
        std::cout << "Failed to open the file! " << inputFileName.c_str() << std::endl;
        cleanTemp();
        return 1;
    }

    num_files = mz_zip_reader_get_num_files(&zip_archive); // Get the number of files
    for (int i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            std::cout << "Failed to read file stat for file " << i << std::endl;
            mz_zip_reader_end(&zip_archive);
            cleanTemp();
            return 1;
        }

        void *file_data = malloc(static_cast<size_t>(file_stat.m_uncomp_size));
        if (!file_data) {
            std::cout << "Failed to allocate memory for file " << i << std::endl;
            mz_zip_reader_end(&zip_archive);
            cleanTemp();
            return 1;
        }

        if (!mz_zip_reader_extract_to_mem(&zip_archive, i, file_data, static_cast<size_t>(file_stat.m_uncomp_size),
                                          0)) {
            std::cout << "Failed to extract file " << i << std::endl;
            free(file_data);
            mz_zip_reader_end(&zip_archive);
            cleanTemp();
            return 1;
        }

        // Create a file in the temp directory and write the extracted data to it
        // All files go to root of the temp directory
        std::string fileWithoutPath(file_stat.m_filename);
        fileWithoutPath = fileWithoutPath.substr(fileWithoutPath.find_last_of("/") + 1);
        std::experimental::filesystem::path tempFilePath = tempPath / fileWithoutPath.c_str();
        std::ofstream outputFile(tempFilePath, std::ios::binary);
        outputFile.write(reinterpret_cast<const char *>(file_data), file_stat.m_uncomp_size);
        outputFile.close();

        free(file_data);
    }
    mz_zip_reader_end(&zip_archive);
    return 0;
}
#endif //CBZ2PDF_UNZIP_H

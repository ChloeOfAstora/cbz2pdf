#ifndef CBZ2PDF_GREYSCALE_H
#define CBZ2PDF_GREYSCALE_H

void convertToGrayscale(unsigned char* data, int width, int height, int channels, unsigned char* gsdata) {
    for (int i = 0; i < width * height; ++i) {
        // Calculate grayscale value using the weighted method (luma)
        unsigned char gray = 0;

        if (channels == 1 || channels == 2) {
            // Grayscale image, no need for conversion
            gray = data[i];
            if (channels == 2) {
                // Grayscale image, no need for conversion
                gsdata[i] = data[i + 1];
            }
        } else if (channels >= 3) {
            // RGB or RGBA image
            unsigned char r = data[i * channels];
            unsigned char g = data[i * channels + 1];
            unsigned char b = data[i * channels + 2];

            gray = static_cast<unsigned char>(0.21f * r + 0.72f * g + 0.07f * b);

            if (channels == 4) {
                // Alpha channel exists, preserve it
                unsigned char alpha = data[i * channels + 3];
                gsdata[i + 1] = alpha;
            }
        }
        gsdata[i] = gray;

    }
}
#endif //CBZ2PDF_GREYSCALE_H

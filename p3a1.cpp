#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
unsigned char clip(const int value) {
    return (value < 0) ? 0 : (value > 255) ? 255 : (unsigned char)value;
}
// Function to perform Floyd-Steinberg dithering on a single channel
void FloydSteinbergDithering(unsigned char** channel, int height, int width) {
    int quant_error;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char oldPixel = channel[y][x];
            unsigned char newPixel = (oldPixel < 128) ? 0 : 255;
            channel[y][x] = newPixel;
            quant_error = oldPixel - newPixel;

            if (x+1 < width) channel[y][x+1] = clip(channel[y][x+1] + quant_error * 7 / 16);
            if (x-1 >= 0 && y+1 < height) channel[y+1][x-1] = clip(channel[y+1][x-1] + quant_error * 3 / 16);
            if (y+1 < height) channel[y+1][x] = clip(channel[y+1][x] + quant_error * 5 / 16);
            if (x+1 < width && y+1 < height) channel[y+1][x+1] = clip(channel[y+1][x+1] + quant_error * 1 / 16);
        }
    }
}

int main(int argc, char *argv[]) {
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 3; // Now we have 3 bytes per pixel, RGB format
    int Width = 500;
    int Height = 375;

    // Allocate image data array dynamically
    unsigned char*** Imagedata = new unsigned char**[Height];
    for (int i = 0; i < Height; ++i) {
        Imagedata[i] = new unsigned char*[Width];
        for (int j = 0; j < Width; ++j) {
            Imagedata[i][j] = new unsigned char[BytesPerPixel];
        }
    }

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            fread(Imagedata[i][j], sizeof(unsigned char), BytesPerPixel, file);
        }
    }
    fclose(file);

    unsigned char** channelC = new unsigned char*[Height];
    unsigned char** channelM = new unsigned char*[Height];
    unsigned char** channelY = new unsigned char*[Height];

    for (int i = 0; i < Height; ++i) {
        channelC[i] = new unsigned char[Width];
        channelM[i] = new unsigned char[Width];
        channelY[i] = new unsigned char[Width];

        for (int j = 0; j < Width; ++j) {
            // Convert RGB to CMY
            channelC[i][j] = 255 - Imagedata[i][j][0];
            channelM[i][j] = 255 - Imagedata[i][j][1];
            channelY[i][j] = 255 - Imagedata[i][j][2];
        }
    }

    // Perform Floyd-Steinberg dithering on each channel
    FloydSteinbergDithering(channelC, Height, Width);
    FloydSteinbergDithering(channelM, Height, Width);
    FloydSteinbergDithering(channelY, Height, Width);

    // Write image data (filename specified by second argument) from image data matrix
    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            unsigned char rgbPixel[3]; // Array to hold RGB values
            // Convert dithered CMY back to RGB
            rgbPixel[0] = 255 - channelC[i][j]; // R
            rgbPixel[1] = 255 - channelM[i][j]; // G
            rgbPixel[2] = 255 - channelY[i][j]; // B

            // Write the RGB data
            fwrite(rgbPixel, sizeof(unsigned char), BytesPerPixel, file);
        }
    }
    fclose(file);

    // Deallocate image data array
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            delete[] Imagedata[i][j];
        }
        delete[] Imagedata[i];
    }
    delete[] Imagedata;

    return 0;
}

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h> 

using namespace std;

// Function to convert RGB to grayscale
void RGBtoGrayscale(unsigned char* RGB, unsigned char* Gray, int Width, int Height) {
    int Pixels = Width * Height;
    for(int i = 0; i < Pixels; i++) {
        int j = i * 3;
        Gray[i] = (unsigned char)(0.2989 * RGB[j] + 0.5870 * RGB[j + 1] + 0.1140 * RGB[j + 2]);
    }
}
// funtion to apply thereshold
unsigned char findThreshold(unsigned char* Gradient, int Width, int Height, float percentile) {
    int histogram[256] = {0};
    int totalPixels = Width * Height;

    for (int i = 0; i < totalPixels; i++) {
        histogram[Gradient[i]]++;
    }

    int cumHistogram[256] = {0};
    cumHistogram[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cumHistogram[i] = cumHistogram[i - 1] + histogram[i];
    }

    int target = int(percentile * totalPixels);
    for (int i = 0; i < 256; i++) {
        if (cumHistogram[i] >= target) {
            return i; 
        }
    }

    return 255; 
}
void Thresholding(unsigned char* Gradient, unsigned char* EdgeMap, int Width, int Height, unsigned char Threshold) {
    int Pixels = Width * Height;
    for (int i = 0; i < Pixels; i++) {
        EdgeMap[i] = (Gradient[i] >= Threshold) ? 255 : 0;
    }
}

// Function to apply Sobel filter and normalize the gradient
void SobelFilter(unsigned char* Gray, unsigned char* Gradient, int Width, int Height) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int maxVal = 0;

    for(int x = 1; x < Height - 1; x++) {
        for(int y = 1; y < Width - 1; y++) {
            float sumX = 0.0;
            float sumY = 0.0;

            // Apply kernels to the image
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    sumX += Gray[(x + i) * Width + (y + j)] * Gx[i + 1][j + 1];
                    sumY += Gray[(x + i) * Width + (y + j)] * Gy[i + 1][j + 1];
                }
            }

            // Calculate the gradient magnitude
            unsigned char magnitude = (unsigned char)sqrt(pow(sumX, 2.0) + pow(sumY, 2.0));
            Gradient[x * Width + y] = magnitude;

            if(magnitude > maxVal) {
                maxVal = magnitude;
            }
        }
    }

    // Normalize the gradient to 0-255
    for(int i = 0; i < Width * Height; i++) {
        Gradient[i] = (Gradient[i] * 255) / maxVal;
    }
}

int main(int argc, char *argv[]) {
    // Define file pointer and variables
    FILE *file;
    int Width = 481, Height = 321;

    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw" << endl;
        return 0;
    }

    // Allocate memory for image data array dynamically
    unsigned char* Imagedata = new unsigned char[Width * Height * 3]; // For RGB
    unsigned char* GrayData = new unsigned char[Width * Height]; // For grayscale
    unsigned char* Gradient = new unsigned char[Width * Height]; // For gradient

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Width * Height * 3, file);
    fclose(file);

    // Convert RGB to Grayscale
    RGBtoGrayscale(Imagedata, GrayData, Width, Height);

    // Apply Sobel Filter
    SobelFilter(GrayData, Gradient, Width, Height);
    
    unsigned char* EdgeMap = new unsigned char[Width * Height];

    float percentile = 0.93; // percentage threshold
    unsigned char Threshold = findThreshold(Gradient, Width, Height, percentile);

// apply the threshold
    Thresholding(Gradient, EdgeMap, Width, Height, Threshold);

    // Write Gradient image data (filename specified by second argument) from image data matrix
    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(EdgeMap, sizeof(unsigned char), Width * Height, file);
    fclose(file);

    // Free allocated memory
    delete[] Imagedata;
    delete[] GrayData;
    delete[] Gradient;
    delete[] EdgeMap;
    return 0;
}

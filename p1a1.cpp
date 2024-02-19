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

// Function to apply Sobel filter and normalize the gradients
void SobelFilter(unsigned char* Gray, unsigned char* GxNormalized, unsigned char* GyNormalized, int Width, int Height) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int maxValX = 0, maxValY = 0;
    int* GradientX = new int[Width * Height];
    int* GradientY = new int[Width * Height];

    // Apply kernels to the image and find maximum gradient values for normalization
    for(int x = 1; x < Height - 1; x++) {
        for(int y = 1; y < Width - 1; y++) {
            int sumX = 0;
            int sumY = 0;

            // Apply kernels to the image
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    sumX += Gray[(x + i) * Width + (y + j)] * Gx[i + 1][j + 1];
                    sumY += Gray[(x + i) * Width + (y + j)] * Gy[i + 1][j + 1];
                }
            }

            // Store gradients
            GradientX[x * Width + y] = sumX;
            GradientY[x * Width + y] = sumY;

            // Update maximum values
            if(abs(sumX) > maxValX) {
                maxValX = abs(sumX);
            }
            if(abs(sumY) > maxValY) {
                maxValY = abs(sumY);
            }
        }
    }

    // Normalize the gradient values to 0-255 and store in output arrays
    for(int i = 0; i < Width * Height; i++) {
        GxNormalized[i] = (unsigned char)(255.0 * abs(GradientX[i]) / maxValX);
        GyNormalized[i] = (unsigned char)(255.0 * abs(GradientY[i]) / maxValY);
    }

    // Free allocated memory for temporary gradient arrays
    delete[] GradientX;
    delete[] GradientY;
}


int main(int argc, char *argv[]) {
    FILE *file;
    int Width = 481, Height = 321;

    if (argc < 4) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image_x_gradient.raw output_image_y_gradient.raw" << endl;
        return 0;
    }

    unsigned char* Imagedata = new unsigned char[Width * Height * 3];
    unsigned char* GrayData = new unsigned char[Width * Height];
    unsigned char* GradientX = new unsigned char[Width * Height];
    unsigned char* GradientY = new unsigned char[Width * Height];

    // Read image
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Width * Height * 3, file);
    fclose(file);

    // Convert RGB to Grayscale
    RGBtoGrayscale(Imagedata, GrayData, Width, Height);

    // Apply Sobel Filter and Generate Gradient Images
    SobelFilter(GrayData, GradientX, GradientY, Width, Height);

    // Write Gradient X image data
    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(GradientX, sizeof(unsigned char), Width * Height, file);
    fclose(file);

    // Write Gradient Y image data
    if (!(file = fopen(argv[3], "wb"))) {
        cout << "Cannot open file: " << argv[3] << endl;
        exit(1);
    }
    fwrite(GradientY, sizeof(unsigned char), Width * Height, file);
    fclose(file);

    // Free allocated memory
    delete[] Imagedata;
    delete[] GrayData;
    delete[] GradientX;
    delete[] GradientY;

    return 0;
}

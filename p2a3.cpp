#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h> 
#include <cstdlib> // for rand() and srand()
#include <ctime> 
#include <vector>

using namespace std;
std::vector<std::vector<int>> expandBayerMatrix(const std::vector<std::vector<int>>& I_n) {
    int n = I_n.size();
    std::vector<std::vector<int>> I_2n(2 * n, std::vector<int>(2 * n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            I_2n[i][j] = 4 * I_n[i][j] + 1;
            I_2n[i][j + n] = 4 * I_n[i][j] + 2;
            I_2n[i + n][j] = 4 * I_n[i][j] + 3;
            I_2n[i + n][j + n] = 4 * I_n[i][j];
        }
    }

    return I_2n;
}

std::vector<std::vector<int>> createThresholdMatrix(const std::vector<std::vector<int>>& I_n, int max_value = 255) {
    int N = I_n.size();
    std::vector<std::vector<int>> T(N, std::vector<int>(N));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            T[i][j] = static_cast<int>(((I_n[i][j] + 0.5) / (N * N)) * max_value);
        }
    }

    return T;
}

int main(int argc, char *argv[]) {
    // Define file pointer and variables
    srand(time(NULL));

    FILE *file;
    int BytesPerPixel;
    int width = 750;
    int height = 500;
    int Threshold = 128;  //set the Threshold
    
    std::vector<std::vector<int>> I_2 = { {1, 2}, {3, 0} };
    std::vector<std::vector<int>> T_2 = createThresholdMatrix(I_2);

    std::vector<std::vector<int>> I_8 = expandBayerMatrix(expandBayerMatrix(I_2));
    std::vector<std::vector<int>> I_32 = expandBayerMatrix(expandBayerMatrix(I_8));

    std::vector<std::vector<int>> T_8 = createThresholdMatrix(I_8);
    std::vector<std::vector<int>> T_32 = createThresholdMatrix(I_32);

    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    if (argc < 4) {
        BytesPerPixel = 1; // default is grey image
    } else {
        BytesPerPixel = atoi(argv[3]);
    }

    // Allocate image data array
    unsigned char* Imagedata = new unsigned char[width * height * BytesPerPixel]; 
    unsigned char* OutputImagedata = new unsigned char[width * height * BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);
    fclose(file);

    int N = T_2.size(); // Assuming we are using T_8, the 8x8 threshold matrix

    // Loop over each pixel and apply the Bayer dithering algorithm
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Compute the corresponding position in the Bayer matrix
            int bayerRow = i % N;
            int bayerCol = j % N;
            // Dither based on the threshold matrix
            for (int k = 0; k < BytesPerPixel; ++k) {
                int index = (i * width + j) * BytesPerPixel + k;
                // If the pixel value is greater than the threshold, set it to 255, otherwise to 0
                OutputImagedata[index] = (Imagedata[index] > T_2[bayerRow][bayerCol]) ? 255 : 0;
            }
        }
    }

    // Write image data (filename specified by second argument) from image data matrix
    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(OutputImagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);
    fclose(file);

    return 0;
}

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h> 
#include <cstdlib> // for rand() and srand()
#include <ctime> 
using namespace std;

int main(int argc, char *argv[]) {
    // Define file pointer and variables
    srand(time(NULL));

    FILE *file;
    int BytesPerPixel;
    int width = 750;
    int height = 500;
    int Threshold = 128;  //set the Threshold
    

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
    unsigned char Imagedata[width][height][BytesPerPixel];
    unsigned char OutputImagedata[width][height][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);
    fclose(file);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < BytesPerPixel; ++k) {
                int randomThreshold = rand() % 256; // Random threshold for each pixel
                OutputImagedata[i][j][k] = (Imagedata[i][j][k] > randomThreshold) ? 255 : 0;
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

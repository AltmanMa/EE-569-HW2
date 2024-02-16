#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h> 

using namespace std;

int main(int argc, char *argv[]) {
    // Define file pointer and variables
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
    unsigned char* Imagedata = new unsigned char[width * height * BytesPerPixel]; 
    unsigned char* OutputImagedata = new unsigned char[width * height * BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);
    fclose(file);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = (i * width + j) * BytesPerPixel;
            int oldPixel = Imagedata[index];
            int newPixel = (oldPixel < Threshold) ? 0 : 255;
            OutputImagedata[index] = newPixel;
        
            int quant_error = oldPixel - newPixel;

        // Spread the quant_error according to the Stucki matrix
            if (j + 1 < width) Imagedata[index + 1] += quant_error * 8 / 42;
            if (j + 2 < width) Imagedata[index + 2] += quant_error * 4 / 42;
        
            for (int k = -2; k <= 2; ++k) {
                if (i + 1 < height && j + k >= 0 && j + k < width) {
                    int weight = (k == -2 || k == 2) ? 2 : (k == -1 || k == 1) ? 4 : 8;
                    Imagedata[index + width + k] += quant_error * weight / 42;
                }
                if (i + 2 < height && j + k >= 0 && j + k < width) {
                    int weight = (k == -2 || k == 2) ? 1 : (k == -1 || k == 1) ? 2 : 4;
                    Imagedata[index + 2 * width + k] += quant_error * weight / 42;
                }
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

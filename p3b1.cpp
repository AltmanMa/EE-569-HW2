#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
unsigned char clip(const int value) {
    return (value < 0) ? 0 : (value > 255) ? 255 : (unsigned char)value;
}
enum MBVQ {
    CMYW, MYGC, RGMY, KRGB, RGBM, CMGB
};
struct Vertex {
    unsigned char R, G, B;
};

MBVQ pyramidMBVQ(unsigned char R, unsigned char G, unsigned char B) {
    if((R+G) > 255) {
        if((G+B) > 255) {
            if((R+G+B) > 510) return CMYW;
            else return MYGC;
        } else return RGMY;
    } else {
        if(!((G+B) > 255)) {
            if(!((R+G+B) > 255)) return KRGB;
            else return RGBM;
        } else return CMGB;
    }
}

void FindClosestVertex(MBVQ mbvq, unsigned char R, unsigned char G, unsigned char B, unsigned char &newR, unsigned char &newG, unsigned char &newB) {
    int distC=  abs(510 + R - G - B);
    int distM = abs(510 + G - R - B);
    int distG = abs(R + B + 255 - G);
    int distB = abs(R + 255 + G - B);
    int distR = abs(255 + G + B - R);
    int distK = abs(R + G + B);
    int distW = abs(765 - R - G - B);
    int distY = abs(510 + B - R - G);
    switch (mbvq) {
        case CMYW:
            if (distC < distM) {
                if (distC < distY) {
                    if(distC < distW){
                        newR = 0; newG = 255; newB = 255; //Cayon
                    } else{
                        newR = 255; newG = 255; newB = 255; //White
                    }
                } else {
                    if(distY < distW){
                        newR = 255; newG = 255; newB = 0;//yellow
                    } else{
                        newR = 255; newG = 255; newB = 255; //White
                    }
                }
            } else {
                if (distM < distY) {
                    if(distM < distW){
                        newR = 255; newG = 0; newB = 255; // Magenta
                    } else{
                        newR = 255; newG = 255; newB = 255; //White
                    }   
                } else {
                    if(distY < distW){
                        newR = 255; newG = 255; newB = 0; // Yellow
                    } else{
                        newR = 255; newG = 255; newB = 255; //White
                    }   
                }
            }
        break;

        case MYGC:
            if (distC < distM) {
                if (distC < distY) {
                    if(distC < distG){
                        newR = 0; newG = 255; newB = 255; //Cayon
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                } else {
                    if(distY < distG){
                        newR = 255; newG = 255; newB = 0; //yellow
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                }
            } else {
                if (distM < distY) {
                    if(distM < distG){
                        newR = 255; newG = 0; newB = 255; // Magenta
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                } else {
                    if(distY < distG){
                        newR = 255; newG = 255; newB = 0; // Yellow
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                }
            }
        break;

        case RGMY:
            if (distR < distM) {
                if (distR < distY) {
                    if(distR < distG){
                        newR = 255; newG = 0; newB = 0; //Red
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                } else {
                    if(distY < distG){
                        newR = 255; newG = 255; newB = 0;//yellow
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                }
            } else {
                if (distM < distY) {
                    if(distM < distG){
                        newR = 255; newG = 0; newB = 255; // Magenta
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                } else {
                    if(distY < distG){
                        newR = 255; newG = 255; newB = 0; // Yellow
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                }
            }
        break;

        case KRGB:
            if (distR < distK) {
                if (distR < distB) {
                    if(distR < distG){
                        newR = 255; newG = 0; newB = 0; //Red
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                } else {
                    if(distB < distG){
                        newR = 0; newG = 0; newB = 255;//Blue
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                }
            } else {
                if (distK < distB) {
                    if(distK < distG){
                        newR = 255; newG = 255; newB = 255; // Black
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                } else {
                    if(distB < distG){
                        newR = 0; newG = 0; newB = 255;//Blue
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                }
            }
        break;

        case RGBM:
            if (distR < distM) {
                if (distR < distB) {
                    if(distR < distG){
                        newR = 255; newG = 0; newB = 0; //Red
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                } else {
                    if(distB < distG){
                        newR = 0; newG = 0; newB = 255;//Blue
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                }
            } else {
                if (distM < distB) {
                    if(distM < distG){
                        newR = 255; newG = 0; newB = 255; // Magenta
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                } else {
                    if(distB < distG){
                        newR = 0; newG = 0; newB = 255;//Blue
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                }
            }
        break;

        case CMGB:
            if (distC < distM) {
                if (distC < distB) {
                    if(distC < distG){
                        newR = 0; newG = 255; newB = 255; //Cayon
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                } else {
                    if(distB < distG){
                        newR = 0; newG = 0; newB = 255;//Blue
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }
                }
            } else {
                if (distM < distB) {
                    if(distM < distG){
                        newR = 255; newG = 0; newB = 255; // Magenta
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                } else {
                    if(distB < distG){
                        newR = 0; newG = 0; newB = 255;//Blue
                    } else{
                        newR = 0; newG = 255; newB = 0; //Green
                    }   
                }
            }
            break;
    }
}

void FloydSteinbergDithering(unsigned char** channelR, unsigned char** channelG, unsigned char** channelB, int height, int width) {
    int quant_errorR, quant_errorG, quant_errorB;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get the current pixel's RGB value
            unsigned char oldPixelR = channelR[y][x];
            unsigned char oldPixelG = channelG[y][x];
            unsigned char oldPixelB = channelB[y][x];

            // Determine the current pixel's MBVQ region
            MBVQ mbvq = pyramidMBVQ(oldPixelR, oldPixelG, oldPixelB);

            unsigned char newPixelR, newPixelG, newPixelB;
            FindClosestVertex(mbvq, oldPixelR, oldPixelG, oldPixelB, newPixelR, newPixelG, newPixelB);


            // Set the new pixel's value
            channelR[y][x] = newPixelR;
            channelG[y][x] = newPixelG;
            channelB[y][x] = newPixelB;

            // Calculate the quantization error for each color channel
            quant_errorR = oldPixelR - newPixelR;
            quant_errorG = oldPixelG - newPixelG;
            quant_errorB = oldPixelB - newPixelB;

            // Diffuse the quantization error to the neighboring pixels
            if (x + 1 < width) {
                channelR[y][x + 1] = clip(channelR[y][x + 1] + quant_errorR * 7 / 16);
                channelG[y][x + 1] = clip(channelG[y][x + 1] + quant_errorG * 7 / 16);
                channelB[y][x + 1] = clip(channelB[y][x + 1] + quant_errorB * 7 / 16);
            }
            if (x - 1 >= 0 && y + 1 < height) {
                channelR[y + 1][x - 1] = clip(channelR[y + 1][x - 1] + quant_errorR * 3 / 16);
                channelG[y + 1][x - 1] = clip(channelG[y + 1][x - 1] + quant_errorG * 3 / 16);
                channelB[y + 1][x - 1] = clip(channelB[y + 1][x - 1] + quant_errorB * 3 / 16);
            }
            if (y + 1 < height) {
                channelR[y + 1][x] = clip(channelR[y + 1][x] + quant_errorR * 5 / 16);
                channelG[y + 1][x] = clip(channelG[y + 1][x] + quant_errorG * 5 / 16);
                channelB[y + 1][x] = clip(channelB[y + 1][x] + quant_errorB * 5 / 16);
            }
            if (x + 1 < width && y + 1 < height) {
                channelR[y + 1][x + 1] = clip(channelR[y + 1][x + 1] + quant_errorR * 1 / 16);
                channelG[y + 1][x + 1] = clip(channelG[y + 1][x + 1] + quant_errorG * 1 / 16);
                channelB[y + 1][x + 1] = clip(channelB[y + 1][x + 1] + quant_errorB * 1 / 16);
            }
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

    unsigned char** channelR = new unsigned char*[Height];
    unsigned char** channelG = new unsigned char*[Height];
    unsigned char** channelB = new unsigned char*[Height];

    for (int i = 0; i < Height; ++i) {
        channelR[i] = new unsigned char[Width];
        channelG[i] = new unsigned char[Width];
        channelB[i] = new unsigned char[Width];

        for (int j = 0; j < Width; ++j) {
            channelR[i][j] = clip(Imagedata[i][j][0]);
            channelG[i][j] = clip(Imagedata[i][j][1]);
            channelB[i][j] = clip(Imagedata[i][j][2]);
        }
    }

    // Perform Floyd-Steinberg dithering on each channel
    FloydSteinbergDithering(channelR,channelG,channelB, Height, Width);

    // Write image data (filename specified by second argument) from image data matrix
    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            unsigned char rgbPixel[3]; // Array to hold RGB values
            rgbPixel[0] = channelR[i][j]; // R
            rgbPixel[1] = channelG[i][j]; // G
            rgbPixel[2] = channelB[i][j]; // B

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

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "error: inputfile + outputfile" << endl;
        return -1;
    }

    // read filenames
    char* inputFile = argv[1];
    char* outputFile = argv[2];

    Mat img = imread(inputFile, IMREAD_GRAYSCALE);

    if(img.empty()) {
        cout << "cannot open file " << inputFile << endl;
        return -1;
    }

    Mat edges;

    // use canny
    Canny(img, edges, 100, 250); 

    bool isSuccess = imwrite(outputFile, edges);
    if (!isSuccess) {
        cout << "fail to save: " << outputFile << endl;
        return -1;
    }

    cout << "process success, save to: " << outputFile << endl;

    return 0;
}

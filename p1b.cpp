#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    // 检查命令行参数数量是否正确
    if (argc != 3) {
        cout << "使用方法: " << argv[0] << " <输入图像文件名> <输出图像文件名>" << endl;
        return -1;
    }

    // 读取用户输入的文件名
    char* inputFile = argv[1];
    char* outputFile = argv[2];

    // 加载图像
    Mat img = imread(inputFile, IMREAD_GRAYSCALE);

    if(img.empty()) {
        cout << "无法加载图像: " << inputFile << endl;
        return -1;
    }

    // 定义结果变量
    Mat edges;

    // 使用Canny边缘检测
    Canny(img, edges, 100, 200); // 这里的100和200是双阈值参数

    // 保存结果图像
    bool isSuccess = imwrite(outputFile, edges);
    if (!isSuccess) {
        cout << "保存图像失败: " << outputFile << endl;
        return -1;
    }

    cout << "图像处理成功，结果已保存到: " << outputFile << endl;

    return 0;
}

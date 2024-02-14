#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    // 加载图像
    Mat img_tiger = imread("Tiger.jpg", IMREAD_GRAYSCALE);
    Mat img_pig = imread("Pig.jpg", IMREAD_GRAYSCALE);

    if(img_tiger.empty() || img_pig.empty()) {
        cout << "无法加载图像!" << endl;
        return -1;
    }

    // 定义结果变量
    Mat edges_tiger, edges_pig;

    // 使用Canny边缘检测
    Canny(img_tiger, edges_tiger, 100, 200); // 这里的100和200是双阈值参数
    Canny(img_pig, edges_pig, 100, 200);

    // 显示结果
    imshow("Edges of Tiger", edges_tiger);
    imshow("Edges of Pig", edges_pig);

    waitKey(0); // 等待用户按键

    return 0;
}

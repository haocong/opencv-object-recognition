//
//  shape.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/5.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    Mat img_1 = imread("/Users/haocong/Desktop/colorFiltered/4.jpg", 0);
//    Mat f;
    Mat img_2 = imread("/Users/haocong/Desktop/colorFiltered/8.jpg", 0);
//    Mat f1;
//    threshold(k,f,50,255,THRESH_BINARY);//对图像进行二值化
//    threshold(k1,f1,50,255,THRESH_BINARY);
    Mat closerect = getStructuringElement(MORPH_RECT, Size(3,3)); //进行结构算子生成
    morphologyEx(img_1, img_1, MORPH_OPEN, closerect);
    morphologyEx(img_2, img_2, MORPH_OPEN, closerect);//进行形态学开运算
//    Mat dst = Mat::zeros(img_1.rows, img_1.cols, CV_8UC3);
//    Mat dst1 = Mat::zeros(img_2.rows, img_2.cols, CV_8UC3);
    vector<vector<Point>> w, w1;
    vector<Vec4i> hierarchy, hierarchy1;
    findContours(img_1, w, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//提取轮廓元素
    findContours(img_2, w1,hierarchy1, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    double ffff = matchShapes(w[0], w1[0], CV_CONTOURS_MATCH_I3, 1.0);//进行轮廓匹配
    cout << ffff << endl;
    return 0;
}

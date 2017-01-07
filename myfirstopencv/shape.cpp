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

int shape(int argc, char** argv)
{
    Mat img_1 = imread("/Users/haocong/Desktop/colorFiltered/red_sample.jpg", 0);
    Mat img_2 = imread("/Users/haocong/Desktop/colorFiltered/red_rotate.jpg", 0);
    
//    threshold(k,f,50,255,THRESH_BINARY); //对图像进行二值化
//    threshold(k1,f1,50,255,THRESH_BINARY);
    
    Mat closerect = getStructuringElement(MORPH_RECT, Size(3,3)); //进行结构算子生成
    
    morphologyEx(img_1, img_1, MORPH_OPEN, closerect);
    morphologyEx(img_2, img_2, MORPH_OPEN, closerect); //进行形态学运算

    vector<vector<Point>> contours_1, contours_2;
    vector<Vec4i> hierarchy1, hierarchy2;
    
    findContours(img_1, contours_1, hierarchy1, RETR_CCOMP, CHAIN_APPROX_NONE); //提取轮廓元素
    findContours(img_2, contours_2, hierarchy2, RETR_CCOMP, CHAIN_APPROX_NONE);
    
    Mat result(img_1.size(), CV_8U, Scalar(0));
    drawContours(result, contours_1, -1, Scalar(255), 2);
    namedWindow("FullScreen",CV_WINDOW_NORMAL);
    imshow("FullScreen", img_1);
    
    double area_1 = 0.0;
    double area_2 = 0.0;
    
    auto itr = contours_1.begin();
    while(itr != contours_1.end())
    {
        area_1 += contourArea(*itr);
        ++itr;
    }
    
    itr = contours_2.begin();
    while(itr != contours_2.end())
    {
        area_2 += contourArea(*itr);
        ++itr;
    }
    
//    double similarity = 0.0;
//    int i = 0;
//    
//    while(i != contours_1.size() || i != contours_2.size())
//    {
//        similarity += matchShapes(contours_1[i], contours_2[i], CV_CONTOURS_MATCH_I3, 1.0);
//        ++i;
//    }
    
//    double similarity = matchShapes(contours_1[0], contours_2[0], CV_CONTOURS_MATCH_I3, 1.0); //进行轮廓匹配
    
    cout << area_1 << ", " << area_2 << ", " << area_2 / area_1 << endl;
//    cout << similarity << endl;
    
    waitKey(0);

    return 0;
}

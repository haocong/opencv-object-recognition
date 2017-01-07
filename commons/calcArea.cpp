//
//  calcArea.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/7.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <opencv2/opencv.hpp>

using namespace cv;

double calcArea(Mat& img)
{
    double area = 0.0;
    Mat closerect = getStructuringElement(MORPH_RECT, Size(3, 3));
    
    morphologyEx(img, img, MORPH_OPEN, closerect);
    
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
    
    auto itr = contours.begin();
    
    while(itr != contours.end())
    {
        area += contourArea(*itr);
        ++itr;
    }
    
    return area;
}

//
//  threshold.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/7.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

Mat GetThreshold(Mat& img, vector<int>& vec)
{
    Mat imgHSV, imgThresholded;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);
    inRange(imgHSV, Scalar(vec[0], vec[2], vec[4]), Scalar(vec[1], vec[3], vec[5]), imgThresholded);
    
    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    
    //morphological closing (fill small holes in the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    return imgThresholded;
}

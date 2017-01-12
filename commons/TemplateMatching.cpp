//
//  TemplateMatching.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/9.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

Point templMatch(Mat& img, Mat& templ)
{
    Mat result;
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    
    result.create(result_rows, result_cols, CV_32FC1);
    matchTemplate(img, templ, result, CV_TM_SQDIFF);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    
    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    
    return minLoc;
}

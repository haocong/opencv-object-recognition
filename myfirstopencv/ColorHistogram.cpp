//
//  ColorHistogram.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/4.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include "ColorHistogram.h"

Mat ColorHistogram::colorReduce(const Mat& image, int div)
{
    Mat outImage;
    int nr = image.rows;
    int nc = image.cols;
    outImage.create(image.size(), image.type());
    if(image.isContinuous() && outImage.isContinuous())
    {
        nr = 1;
        nc = nc * image.rows * image.channels();
    }
    for(int i=0;i<nr;i++)
    {
        const uchar* inData=image.ptr<uchar>(i);
        uchar* outData=outImage.ptr<uchar>(i);
        for(int j = 0; j < nc; j++)
        {
            *outData++ = *inData++ / div * div + div / 2;
        }
    }
    return outImage;
};

MatND ColorHistogram::getHistogram(const MatND& hist)
{
    double maxVal=0;
    double minVal=0;
    
    //找到直方图中的最大值和最小值
    minMaxLoc(hist,&minVal,&maxVal,0,0);
    int histSize=hist.rows;
    Mat histImg(histSize,histSize,CV_8U,Scalar(255));
    // 设置最大峰值为图像高度的90%
    int hpt=static_cast<int>(0.9*histSize);
    
    for(int h=0;h<histSize;h++)
    {
        float binVal=hist.at<float>(h);
        int intensity=static_cast<int>(binVal*hpt/maxVal);
        line(histImg,Point(h,histSize),Point(h,histSize-intensity),Scalar::all(0));
    }
    
    return histImg;
}

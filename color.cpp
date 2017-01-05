//
//  color.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/4.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <cv.h>
#include <highgui.h>

// 该接口来至   冰风2009手势识别
// 进行肤色检测
void SkinDetect(IplImage* src,IplImage* dst)
{
    // 创建图像头
    IplImage* hsv = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3); //用于存图像的一个中间变量，是用来分通道用的，分成hsv通道
    IplImage* tmpH1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);//通道的中间变量，用于肤色检测的中间变量
    IplImage* tmpS1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage* tmpH2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage* tmpS2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage* tmpH3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage* tmpS3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    
    IplImage* H = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage* S = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage* V = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage* src_tmp1 = cvCreateImage(cvGetSize(src), 8, 3);
    
    // 高斯模糊
    cvSmooth(src, src_tmp1, CV_GAUSSIAN, 3, 3); //高斯模糊
    
    // hue色度，saturation饱和度，value纯度
    cvCvtColor(src_tmp1, hsv, CV_BGR2HSV );//颜色转换
    cvCvtPixToPlane(hsv, H, S, V, 0);//分为3个通道
    
    /*********************肤色检测部分**************/
    cvInRangeS(H, cvScalar(0.0, 0.0, 0,0), cvScalar(20.0, 0.0, 0,0), tmpH1);
    cvInRangeS(S, cvScalar(75.0, 0.0, 0,0), cvScalar(200.0, 0.0, 0,0), tmpS1);
    cvAnd(tmpH1, tmpS1, tmpH1, 0);
    
    // Red Hue with Low Saturation
    // Hue 0 to 26 degree and Sat 20 to 90
    cvInRangeS(H, cvScalar(0.0,0.0,0,0), cvScalar(13.0,0.0,0,0), tmpH2);
    cvInRangeS(S, cvScalar(20.0,0.0,0,0),cvScalar(90.0,0.0,0,0), tmpS2);
    cvAnd(tmpH2, tmpS2, tmpH2,0);

    // Red Hue to Pink with Low Saturation
    // Hue 340 to 360 degree and Sat 15 to 90
    cvInRangeS(H, cvScalar(170.0, 0.0, 0,0), cvScalar(180.0, 0.0, 0,0), tmpH3);
    cvInRangeS(S, cvScalar(15.0, 0.0, 0,0), cvScalar(90.0, 0.0, 0,0), tmpS3);
    cvAnd(tmpH3, tmpS3, tmpH3, 0);
    
    // Combine the Hue and Sat detections
    cvOr(tmpH3, tmpH2, tmpH2, 0);
    cvOr(tmpH1, tmpH2, tmpH1, 0);
    
    cvCopy(tmpH1, dst);
    
    cvReleaseImage(&hsv);
    cvReleaseImage(&tmpH1);
    cvReleaseImage(&tmpS1);
    cvReleaseImage(&tmpH2);
    cvReleaseImage(&tmpS2);
    cvReleaseImage(&tmpH3);
    cvReleaseImage(&tmpS3);
    cvReleaseImage(&H);
    cvReleaseImage(&S);
    cvReleaseImage(&V);
    cvReleaseImage(&src_tmp1);
}

int color(int argc, char* argv[])
{
    
    IplImage *src=cvLoadImage("/Users/haocong/Desktop/sample/marker.jpg", CV_LOAD_IMAGE_COLOR);
    //
    cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
    cvShowImage("src", src);
    //
    
    cvNamedWindow("flip", CV_WINDOW_AUTOSIZE);
    IplImage *dsc = cvCreateImage(cvGetSize(src), src->depth, 1);
    
    // 以x轴翻转
    SkinDetect(src, dsc);
    // cvFlip(src, dsc, 1);
    cvShowImage("flip", dsc);
    cvWaitKey(0);
    cvDestroyAllWindows();
    cvReleaseImage(&src);
    printf("Hello World!/n");
    return 0;
}

#include <highgui.h>
#include <cv.h>
#include <opencv2/legacy/legacy.hpp>
using namespace std;

IplImage* doScale(IplImage* image_input, double scale)
{
    CvSize size;
    size.width = (int)image_input->width * scale;
    size.height = (int)image_input->height * scale;
    IplImage* image_output = cvCreateImage(size, image_input->depth, image_input->nChannels);
    cvResize(image_input, image_output, CV_INTER_LINEAR);
    return image_output;
}

IplImage* doCanny(IplImage* image_input,
                  double lowThresh,
                  double highThresh,
                  double aperture)
{
    if(image_input->nChannels != 1)
        return (0);
    
    IplImage* image_output = cvCreateImage(cvGetSize(image_input),
                                           image_input->depth,
                                           image_input->nChannels);
    
    cvCanny(image_input,image_output,lowThresh,highThresh,aperture);
    
    return image_output;
}

int hist()
{
    IplImage *src1 = cvLoadImage("/Users/haocong/Desktop/colorFiltered/3.jpg");
    IplImage *src2 = cvLoadImage("/Users/haocong/Desktop/colorFiltered/6.jpg");
    
    double scale = 320.0 / src1->width;
    
    IplImage *img1 = doScale(src1, scale);
    IplImage *img2 = doScale(src2, scale);
    
    IplImage *img_gray1 = cvCreateImage(cvGetSize(img1),IPL_DEPTH_8U,1);
    IplImage *img_gray2 = cvCreateImage(cvGetSize(img2),IPL_DEPTH_8U,1);
    
    cvCvtColor(img1,img_gray1,CV_BGR2GRAY);
    cvCvtColor(img2,img_gray2,CV_BGR2GRAY);
    
    cvNamedWindow("img1",CV_WINDOW_AUTOSIZE);
    cvShowImage("img1", img1);
    cvNamedWindow("img2",CV_WINDOW_AUTOSIZE);
    cvShowImage("img2", img2);
    
//    IplImage *img_canny1 = doCanny(img_gray1, 50, 100, 3);
//    IplImage *img_canny2 = doCanny(img_gray2, 50, 100, 3);
    
    cvNamedWindow("img_gray1",CV_WINDOW_AUTOSIZE);
    cvShowImage("img_gray1", img_gray1);
    cvNamedWindow("img_gray2",CV_WINDOW_AUTOSIZE);
    cvShowImage("img_gray2", img_gray2);
    
    int hist_size=256;//直方图的横轴长度
    // int hist_height=256;//直方图的纵轴高度
    float range[]={0,255}; //灰度级的范围
    float* ranges[]={range};
    
    CvHistogram *Histogram1 = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
    CvHistogram *Histogram2 = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
    
    cvCalcHist(&img_gray1,Histogram1,0,0);//计算直方图
    cvNormalizeHist(Histogram1,1.0);//归一化直方图
    cvCalcHist(&img_gray2,Histogram2,0,0);//计算直方图
    cvNormalizeHist(Histogram2,1.0);//归一化直方图
    
    printf("CV_COMP_CORREL : %.4f\n",cvCompareHist(Histogram1,Histogram2,CV_COMP_CORREL));
    printf("CV_COMP_CHISQR : %.4f\n",cvCompareHist(Histogram1,Histogram2,CV_COMP_CHISQR));
    printf("CV_COMP_INTERSECT : %.4f\n",cvCompareHist(Histogram1,Histogram2,CV_COMP_INTERSECT));
    printf("CV_COMP_BHATTACHARYYA : %.4f\n",cvCompareHist(Histogram1,Histogram2,CV_COMP_BHATTACHARYYA));
    
    cvWaitKey(0);
    cvDestroyAllWindows();
    cvReleaseImage(&src1);
    cvReleaseImage(&src2);
    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
    cvReleaseImage(&img_gray1);
    cvReleaseImage(&img_gray2);
    
    return 0;
}

//
//  play.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/4.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <opencv2/legacy/legacy.hpp>
using namespace std;
using namespace cv;

string pHashValue(Mat &src)
{
    Mat img ,dst;
    string rst(64,'\0');
    double dIdex[64];
    double mean = 0.0;
    int k = 0;
    if(src.channels()==3)
    {
        cvtColor(src,src,CV_BGR2GRAY);
        img = Mat_<double>(src);
    }
    else
    {
        img = Mat_<double>(src);
    }
    
    /* 第一步，缩放尺寸，可以为Size(32,32)或Size(8,8)，也可以更高，主要是为了提高计算效率*/
    resize(img, img, Size(32,32));
    
    /* 第二步，离散余弦变换，DCT系数求取*/
    dct(img, dst);
    
    /* 第三步，求取DCT系数均值（左上角8*8区块的DCT系数）*/
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
        {
            dIdex[k] = dst.at<double>(i, j);
            mean += dst.at<double>(i, j)/64;
            ++k;
        }
    }
    
    /* 第四步，计算哈希值。*/
    for (int i =0;i<64;++i)
    {
        if (dIdex[i]>=mean)
        {
            rst[i]='1';
        }
        else
        {
            rst[i]='0';
        }
    }
    return rst;
}

int HanmingDistance(string &str1,string &str2)
{
    if((str1.size()!=64)||(str2.size()!=64))
        return -1;
    int difference = 0;
    for(int i=0;i<64;i++)
    {
        if(str1[i]!=str2[i])
            difference++;
    }
    return difference;
}

int play()
{
    Mat k = imread("/Users/haocong/Desktop/sample/2.jpg");//读取图片
    Mat k1 = imread("/Users/haocong/Desktop/sample/8.jpg");
    
    string hash1 = pHashValue(k);
    string hash2 = pHashValue(k1);
    
    cout << HanmingDistance(hash1, hash2) << endl;

//    namedWindow("img");
//    imshow("img",f);//把轮廓画出来
//    namedWindow("img1");
//    imshow("img1",f1);

    return 0;
}

//
//  gif.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/6.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <gif_lib.h>

using namespace std;
using namespace cv;

int gif(int ac, char **av)
{
    GifFileType *f = DGifOpenFileName("/Users/haocong/Desktop/gifs/test.gif");
    assert(f != NULL);
    int ret = DGifSlurp(f);
    assert(ret == GIF_OK);
    int width = f->SWidth;
    int height = f->SHeight;
    
    std::cout << "Total Frames:     " << f->ImageCount << std::endl;
    std::cout << "Image Size:       " << width << " X " << height << std::endl;
    std::cout << "Color Resolution: " << f->SColorResolution << std::endl;
    
    GifColorType *colorMap = f->SColorMap->Colors;
    
    cv::namedWindow("image");
    cv::Mat img(height, width, CV_8UC3);
    
    // Play for 100 frames ...
    for (int t = 0; t < 100; t++) {
        int frameNo = t % f->ImageCount;
        std::cout << frameNo << " / " << f->ImageCount << std::endl;
        SavedImage *image = &f->SavedImages[frameNo];
        auto *ptr = img.ptr<cv::Vec3b>();
        for (int i = 0; i < width * height; i++, ptr++) {
            const GifColorType &color = colorMap[image->RasterBits[i]];
            *ptr = cv::Vec3b(color.Red, color.Green, color.Blue);
        }
        cvtColor(img, img, CV_RGB2BGR);
        imshow("image", img);
        cv::waitKey(1000);
    }
    
    DGifCloseFile(f);
    return 0;
}

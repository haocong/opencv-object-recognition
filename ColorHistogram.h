//
//  ColorHistogram.h
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/4.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <opencv2/core/core.hpp>
using namespace cv;

#ifndef ColorHistogram_h
#define ColorHistogram_h
class ColorHistogram
{
public:
    Mat colorReduce(const Mat&, int);
    MatND getHistogram(const MatND&);
};

#endif /* ColorHistogram_h */

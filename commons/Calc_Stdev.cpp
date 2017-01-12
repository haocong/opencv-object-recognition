//
//  Calc_Stdev.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/10.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <numeric>
#include <cmath>

using namespace cv;

double calcStdev(Vector<int>& resultSet)
{
    double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);
    double mean =  sum / resultSet.size(); //均值
    
    double accum  = 0.0;
    std::for_each (std::begin(resultSet), std::end(resultSet), [&](const double d) {
        accum  += (d-mean)*(d-mean);
    });
    
    double stdev = sqrt(accum / (resultSet.size()-1)); //方差
    return stdev;
}

//
//  surf.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/4.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
using namespace cv;

int surf()
{
    Mat img_1 = imread("/Users/haocong/Desktop/colorFiltered/1.jpg");
    Mat img_2 = imread("/Users/haocong/Desktop/colorFiltered/3.jpg");
    
    int minHessian = 400;
    
    SiftFeatureDetector detector(minHessian);
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    detector.detect(img_1, keypoints_1);
    detector.detect(img_2, keypoints_2);
    
    SiftDescriptorExtractor extractor;
    Mat descriptors_1, descriptors_2;
    
    extractor.compute(img_1, keypoints_1, descriptors_1);
    extractor.compute(img_2, keypoints_2, descriptors_2);
    
    FlannBasedMatcher matcher;
    std::vector<DMatch> matches;
    matcher.match(descriptors_1, descriptors_2, matches);
    
    double max_dist = 0, min_dist = 100;
    
    for(int i = 0; i < descriptors_1.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist) min_dist = dist;
        if(dist > max_dist) max_dist = dist;
    }
    
    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);
    
    std::vector<DMatch> good_matches;
    
    for(int i = 0; i < descriptors_1.rows; i++)
    {
        if(matches[i].distance < 2 * min_dist)
        {
            good_matches.push_back(matches[i]);
        }
    }
    
    Mat img_matches;
    drawMatches(img_1, keypoints_1, img_2, keypoints_2,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imshow("Good Matches", img_matches);
    for( int i = 0; i < good_matches.size(); i++ )
    {
        printf("-- Good Match [%d] Keypoint 1(%f,%f): %d  -- Keypoint 2(%f,%f): %d  \n", i,
               keypoints_1[good_matches[i].queryIdx].pt.x,keypoints_1[good_matches[i].queryIdx].pt.y, good_matches[i].queryIdx,
               keypoints_2[good_matches[i].trainIdx].pt.x,keypoints_2[good_matches[i].trainIdx].pt.y,good_matches[i].trainIdx);
    }
    waitKey(0);
    return 0;
}

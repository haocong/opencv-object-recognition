//
//  Check_Match.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/9.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

double calcStdev(Vector<int>& resultSet);
Point templMatch(Mat&, Mat&);

bool ascend(pair<string, double> a, pair<string, double> b) {
    return a.second < b.second;
}

double calcDist(Point& p1, Point& p2)
{
    double xSquare = (p2.x - p1.x) * (p2.x - p1.x);
    double ySquare = (p2.y - p1.y) * (p2.y - p1.y);
    return sqrt(xSquare + ySquare);
}

pair<string, double> getMostSimilar(Mat& img, Vector<pair<string, Vector<Mat>>>& templates)
{
//    Vector<pair<string, double>> dists;
//    Point img_center(img.cols / 2, img.rows / 2);
//    for (auto itr = templates.begin(); itr != templates.end(); itr++)
//    {
//        pair<string, Vector<Mat>> side = *itr;
//        Mat target = side.second[4];
//        Point matchLoc = templMatch(img, target);
//        matchLoc.x += target.cols / 2;
//        matchLoc.y += target.rows / 2;
//        double dist = calcDist(matchLoc, img_center);
//        dists.push_back(make_pair(side.first, dist));
//    }
//    sort(dists.begin(), dists.end(), ascend);
//    return dists[0];
    int counts = (int)templates[0].second.size();
    int order = (int)sqrt(counts);
    Vector<pair<string, double>> stdevs;
    
    for (auto itr = templates.begin(); itr != templates.end(); itr++)
    {
        pair<string, Vector<Mat>> side = *itr;
        Vector<Point> matchPoints;
        Vector<int> gaps;
        for (int i = 0; i < counts; i += order + 1)
        {
            Point matchLoc = templMatch(img, side.second[i]);
            matchPoints.push_back(matchLoc);
        }
        for(auto jtr = matchPoints.begin() + 1; jtr != matchPoints.end(); jtr++)
        {
            gaps.push_back(abs(jtr->x - (jtr - 1)->x - side.second[0].cols));
            gaps.push_back(abs(jtr->y - (jtr - 1)->y - side.second[0].rows));
        }
        stdevs.push_back(make_pair(side.first, calcStdev(gaps)));
    }
    sort(stdevs.begin(), stdevs.end(), ascend);
    return stdevs[0];
}

bool checkColGap(Vector<Point>& points, double squareSize, double threshold)
{
    int pointCounts = (int)points.size();
    int diff = 0;
    
    for (int i = 1; i < pointCounts; i++)
    {
        diff = points[i].y - points[i - 1].y;
        if (diff < 0 || abs(diff - squareSize) > squareSize * threshold) {
            return false;
        }
    }
    return true;
}

bool checkRowGap(Vector<Point>& points, double squareSize, double threshold)
{
    int pointCounts = (int)points.size();
    int diff = 0;
    
    for (int i = 1; i < pointCounts; i++)
    {
        diff = points[i].x - points[i - 1].x;
        if (diff < 0 || abs(diff - squareSize) > squareSize * threshold) {
            return false;
        }
    }
    return true;
}

bool checkMatch(Vector<Point>& points, Mat& templ, double threshold)
{
    int counts = (int)points.size();
    int order = (int)sqrt(counts);
    Vector<int> gaps;
    int diff = 0;

    for (int i = 0; i < order; i++)
    {
        Vector<Point> rowPoints, colPoints;
        for (int j = i * order + 1; j < (i + 1) * order; j++)
        {
//            rowPoints.push_back(points[j]);
            diff = points[j].x - points[j - 1].x - templ.cols;
            if(diff <= 0)
                return false;
            gaps.push_back(diff);
            
        }
        for (int k = i + order; k < counts; k += order)
        {
//            colPoints.push_back(points[k]);
            diff = points[k].y - points[k - order].y - templ.rows;
            if(diff <= 0)
                return false;
            gaps.push_back(diff);
        }
//        if (!checkRowGap(rowPoints, squareSize, threshold) || !checkColGap(colPoints, squareSize, threshold))
//            return false;
    }
    return calcStdev(gaps) < threshold;
}


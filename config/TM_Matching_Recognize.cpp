//
//  TM_Matching_Recognize.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/9.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "json.hpp"

using namespace cv;
using namespace std;
using namespace nlohmann;

Point templMatch(Mat&, Mat&);
bool checkMatch(Vector<Point>&, Mat&, double);

pair<string, Vector<Point>> patternMatch(Vector<pair<string, Vector<Mat>>>& templates, Mat& imgOriginal, int templateCounts, double threshold)
{
    for (auto itr = templates.begin(); itr != templates.end(); itr++)
    {
        pair<string, Vector<Mat>> patterns = *itr;
        Vector<Mat> templs = patterns.second;
        Vector<Point> matchLocs;
        for (auto jtr = templs.begin(); jtr != templs.end(); jtr++)
        {
            Mat templ = *jtr;
            Point matchLoc = templMatch(imgOriginal, templ);
            matchLocs.push_back(matchLoc);
        }
        if ((int)matchLocs.size() == templateCounts &&
            checkMatch(matchLocs, templs[0], threshold))
        {
            return make_pair(patterns.first, matchLocs);
        }
    }
    return make_pair("", NULL);
}

void rotateNinety(Mat& imgInput, Mat& imgOutput)
{
    transpose(imgInput, imgOutput);
    flip(imgInput, imgOutput, 0);
}

int main(int argc, char** argv)
{
    ifstream i(argv[2]);
    json config;
    i >> config;
    
    VideoCapture cap(config["cameraID"].get<int>()); //capture the video from web cam
    
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, config["frameWidth"].get<double>());
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, config["frameHeight"].get<double>());

    bool loop = true;
    Mat imgOriginal;
    double imgRatio = 0.0;
    Vector<pair<string, Vector<Mat>>> templates;
    
    double threshold = config["threshold"].get<double>();
    string patternBaseDir = config["baseDir"].get<string>() + "/patterns";
    int patternCounts = (int)config["patterns"].size();
    int templateCounts = (int)config["matrixOrder"] * (int)config["matrixOrder"];
    
    for(int i = 0; i < patternCounts; i++)
    {
        Vector<Mat> templVec;
        json pattern = config["patterns"].at(i);
        string patternName = pattern["name"];
        
        for (int j = 1; j < templateCounts + 1; j++)
        {
            Mat templ = imread(patternBaseDir + "/" + patternName + "/" + to_string(j) + ".jpg");
            templVec.push_back(templ);
        }
        
        templates.push_back(make_pair(patternName, templVec));
    }
    
    while (loop)
    {
//        imgOriginal = imread("/Users/haocong/Desktop/sample/1.jpg");
        
        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
        
        if (!imgRatio)
        {
            imgRatio = (double) imgOriginal.cols / imgOriginal.rows;
        }
        
        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        
        for (int i = 0; i < 1; i++)
        {
            if (i > 0)
            {
                rotateNinety(imgOriginal, imgOriginal);
            }
            pair<string, Vector<Point>> result = patternMatch(templates, imgOriginal, templateCounts, threshold);
            if (!result.first.empty())
            {
                cout << result.first << endl;
                Vector<Point> matchPoints = result.second;
                for (auto itr = matchPoints.begin(); itr != matchPoints.end(); itr++)
                {
                    Mat templ = templates[0].second[0];
                    rectangle(imgOriginal, *itr, Point(itr->x + templ.cols , itr->y + templ.rows), Scalar::all(255), 2, 8, 0 );
                }
                break;
            }
        }
        
        resize(imgOriginal, imgOriginal, Size(320, (int) 320 / imgRatio));
        imshow("result", imgOriginal); //show the original image
        
        switch (waitKey(1)) {
            case 13:
            {
                break;
            }
            case 27:
            {
                loop = false;
                cout << "Exit Recognition." << endl;
                break;
            }
            default:
                break;
        }

    }
        
    return 0;
}

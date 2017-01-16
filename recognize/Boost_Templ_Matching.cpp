//
//  Boost_Templ_Matching.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/11.
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

void rotateNinety(Mat& imgInput, Mat& imgOutput)
{
    transpose(imgInput, imgOutput);
    flip(imgInput, imgOutput, 0);
}

Point templMatch(Mat&, Mat&);
bool checkMatch(Vector<Point>&, Mat&, double);

bool ascend(pair<string, double> a, pair<string, double> b);

Vector<Point> patternMatch(Mat& img, Vector<Mat>& patterns, int templateCounts, double threshold)
{
    Vector<Point> matchLocs;
    for (auto itr = patterns.begin(); itr != patterns.end(); itr++)
    {
        Mat templ = *itr;
        Point matchLoc = templMatch(img, templ);
        matchLocs.push_back(matchLoc);
    }
    if ((int)matchLocs.size() == templateCounts &&
        checkMatch(matchLocs, patterns[0], threshold))
        return matchLocs;
    return NULL;
}

pair<string, double> getMostSimilar(Mat& img, Vector<pair<string, Vector<Mat>>>& templates);

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
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, config["frameWidth"].get<int>());
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, config["frameHeight"].get<int>());
    
    bool loop = true;
    Mat imgOriginal;
    double imgRatio = 0.0;
    Vector<pair<string, Vector<Mat>>> templates;
    
    double threshold = config["threshold"].get<double>();
    string patternBaseDir = config["baseDir"].get<string>() + "/patterns";
    int patternCounts = (int)config["patterns"].size();
    int templateCounts = (int)config["matrixOrder"] * (int)config["matrixOrder"];

    for (int i = 0; i < patternCounts; i++)
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
    
    while(loop)
    {
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
        
        Vector<pair<string, double>> dists, dists_copy;
        Vector<Mat> originals;
        
        for (int i = 0; i < 4; i++)
        {
            originals.push_back(imgOriginal);
            pair<string, double> mostSimilar = getMostSimilar(imgOriginal, templates);
            dists.push_back(mostSimilar);
            dists_copy.push_back(mostSimilar);
            rotateNinety(imgOriginal, imgOriginal);
        }
        
        sort(dists.begin(), dists.end(), ascend);
        
        int pos = 0;
        
        for (; pos < 4; pos++)
        {
            if (dists_copy[pos] == dists[0])
                break;
        }
        
        Mat imgRotated = originals[pos];
        string pattern = dists[0].first;
        Vector<Mat> patterns;
        
        for(auto itr = templates.begin(); itr != templates.end(); itr++)
        {
            if(itr->first == pattern)
            {
                patterns = itr->second;
            }
        }

        Vector<Point> matchPoints = patternMatch(imgRotated, patterns, templateCounts, threshold);
        if (matchPoints.size() != 0)
        {
            cout << pattern << endl;
            for (auto itr = matchPoints.begin(); itr != matchPoints.end(); itr++)
            {
                Mat templ = templates[0].second[0];
//                cout << itr->x << " " << itr->y << endl;
                rectangle(imgRotated, *itr, Point(itr->x + templ.cols , itr->y + templ.rows), Scalar::all(255), 2, 8, 0 );
            }
            for (int i = 0; i < 4 - pos; i++)
            {
                rotateNinety(imgRotated, imgRotated);
            }
            imgOriginal = imgRotated;
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

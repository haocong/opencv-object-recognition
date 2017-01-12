//
//  main.cpp
//  recognize
//
//  Created by haocong on 2017/1/7.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "json.hpp"

using namespace std;
using namespace cv;
using namespace nlohmann;

Mat GetThreshold(Mat&, vector<int>&);
double calcArea(Mat&);

bool descend(pair<string, double> a, pair<string, double> b) {
    return a.second > b.second;
}

int colorRecg(int argc, char** argv)
{
    VideoCapture cap(1); //capture the video from web cam
    
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640.0);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480.0);
    cap.set(CV_CAP_PROP_FPS, 30.0);
    cap.set(CV_CAP_PROP_EXPOSURE, 0.1);
    
    ifstream i("./config.json");
    json config;
    i >> config;
    
    while (true) {
        Mat img;
        Mat imgThreshold;
        
        bool bSuccess = cap.read(img); // read a new frame from video
        
        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        
        vector<pair<string, double>> sims;
        
        for (json::iterator it = config.begin(); it != config.end(); ++it) {
            json color = *it;
            vector<int> vec = color["range"].get<vector<int>>();
            imgThreshold = GetThreshold(img, vec);
            double area = calcArea(imgThreshold);
            double similarity = area / color["area"].get<double>();
            //        cout << it.key() << ": " << similarity << endl;
            sims.push_back(make_pair(it.key(), similarity));
        }
        
        imshow("Image", img);
        
        sort(sims.begin(), sims.end(), descend);
        pair<string, double> bestSim = sims[0];
        //    cout << bestSim.first << ": " << bestSim.second << endl;
        
        if(bestSim.second > 0.85) {
            cout << bestSim.first << " " << bestSim.second << endl;
        } else {
            cout << "nomatch " << bestSim.first << " " << bestSim.second << endl;
        }
        
        waitKey(33);
    }
    
    return 0;
}

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
#include <unordered_map>
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

int main(int argc, char** argv)
{
    ifstream i("/Users/haocong/Desktop/config.json");
    json config;
    i >> config;
    
    Mat img = imread("/Users/haocong/Desktop/sample/15.jpg");
    Mat imgThreshold;
    
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
    
    sort(sims.begin(), sims.end(), descend);
    pair<string, double> bestSim = sims[0];
//    cout << bestSim.first << ": " << bestSim.second << endl;
    
    if(bestSim.second > 0.85) {
        cout << bestSim.first << endl;
    }
    return 0;
}

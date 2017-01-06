//
//  main.cpp
//  config
//
//  Created by haocong on 2017/1/7.
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

double calcArea(Mat&);

int main(int argc, char** argv)
{
    
    //    VideoCapture cap(0); //capture the video from web cam
    //
    //    if ( !cap.isOpened() )  // if not success, exit program
    //    {
    //        cout << "Cannot open the web cam" << endl;
    //        return -1;
    //    }
    
    ifstream i("/Users/haocong/Desktop/config.json");
    json config;
    i >> config;
    
    string color;
    
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    
    int iLowH = 35;
    int iHighH = 77;
    
    int iLowS = 65;
    int iHighS = 255;
    
    int iLowV = 65;
    int iHighV = 255;
    
    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 180); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 180);
    
    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);
    
    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
    
    bool loop = true;
    
    while (loop)
    {
        Mat imgOriginal = imread("/Users/haocong/Desktop/sample/3.jpg");
        
        //        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
        //
        //        if (!bSuccess) //if not success, break loop
        //        {
        //            cout << "Cannot read a frame from video stream" << endl;
        //            break;
        //        }
        
        Mat imgHSV;
        
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        
        Mat imgThresholded;
        
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
        
        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image
        
        switch (waitKey(30)) {
            case 13:
            {
                cout << "H: " << iLowH << ", " << iHighH << endl;
                cout << "S: " << iLowS << ", " << iHighS << endl;
                cout << "V: " << iLowV << ", " << iHighV << "\n" << endl;
                cout << "Enter Current Color: " << endl;
                cin >> color;
                if (config.find(color) == config.end()) {
                    cout << "Illegal Color Type!" << endl;
                } else {
                    config[color]["range"][0] = iLowH;
                    config[color]["range"][1] = iHighH;
                    config[color]["range"][2] = iLowS;
                    config[color]["range"][3] = iHighS;
                    config[color]["range"][4] = iLowV;
                    config[color]["range"][5] = iHighV;
                    config[color]["area"] = calcArea(imgThresholded);
                    ofstream o("/Users/haocong/Desktop/config.json");
                    o << setw(2) << config << endl;
                    cout << "Color " << color << " Setted!" << endl;
                }
                break;
            }
            case 27:
            {
                loop = false;
                cout << "Exit Configuration." << endl;
                break;
            }
            default:
                break;
        }
    }
    
    return 0;
    
}

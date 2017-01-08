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
    VideoCapture cap(1); //capture the video from web cam

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640.0);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480.0);
    cap.set(CV_CAP_PROP_FPS, 30.0);
    
    ifstream i("./config.json");
    json config;
    i >> config;
    
    string color;
    
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    
    int iLowH = 0;
    int iHighH = 10;
    
    int iLowS = 43;
    int iHighS = 255;
    
    int iLowV = 46;
    int iHighV = 255;
    
    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 180); //Hue (0 - 180)
    createTrackbar("HighH", "Control", &iHighH, 180);
    
    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);
    
    createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);
    
    bool loop = true;
    
    while (loop)
    {
        Mat imgOriginal = imread("/Users/haocong/Desktop/sample/5.jpg");
        
//        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
//
//        if (!bSuccess) //if not success, break loop
//        {
//            cout << "Cannot read a frame from video stream" << endl;
//            break;
//        }
        
        cout << "H: " << iLowH << ", " << iHighH << endl;
        cout << "S: " << iLowS << ", " << iHighS << endl;
        cout << "V: " << iLowV << ", " << iHighV << "\n" << endl;
        
        Mat imgHSV;
        
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        
        Mat imgThresholded, temp;
        
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
        
        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        
        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        
        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image
        
        switch (waitKey(30)) {
            case 13:
            {
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
                    ofstream o("./config.json");
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

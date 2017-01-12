//
//  Capture.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/10.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "json.hpp"

using namespace cv;
using namespace std;
using namespace nlohmann;

int main(int argc, char** argv)
{
    ifstream i(argv[2]);
    json config;
    i >> config;
    
    Mat imgCapture;
    bool loop = true;
    int counter = 1;
    
    VideoCapture cap(config["cameraID"].get<int>()); //capture the video from web cam
    string savePath = config["baseDir"].get<string>() + "/captures";
    
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, config["frameWidth"].get<double>());
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, config["frameHeight"].get<double>());

    while (loop) {
        bool bSuccess = cap.read(imgCapture); // read a new frame from video
        
        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        
        imshow("frame", imgCapture);
        switch (waitKey(33)) {
            case 13:
            {
                imwrite(savePath + "/" + to_string(counter) + ".jpg", imgCapture);
                counter++;
                cout << "Frame Captured." << endl;
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


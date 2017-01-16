//
//  SURF_Homography.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/9.
//  Copyright © 2017年 nankai. All rights reserved.
//

/**
 * @file MatchTemplate_Demo.cpp
 * @brief Sample code to use the function MatchTemplate
 * @author OpenCV team
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";
string path;

int match_method;
int max_Trackbar = 5;
int imageNum = 4;

/// Function Headers
void MatchingMethod( int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
    VideoCapture cap(1);
    
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    
    path = argv[1];
    
    if(argv[2])
    {
        imageNum = (int) strtol(argv[3], NULL, 10);
    }
    
    /// Create windows
    namedWindow( image_window, WINDOW_AUTOSIZE );
//    namedWindow( result_window, WINDOW_AUTOSIZE );
    
    /// Create Trackbar
    const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );
    
    while (true) {
        bool bSuccess = cap.read(img);
        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        MatchingMethod( 0, 0 );
        if(waitKey(33) == 27)
            break;
    }
    return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
    Mat img_display;
    img.copyTo( img_display );
    for(int i = 0; i < imageNum; i++)
    {
        /// Source image to display
        
        templ = imread(path + "/" + to_string(i + 1) + ".jpg");
        
        /// Create the result matrix
        int result_cols =  img.cols - templ.cols + 1;
        int result_rows = img.rows - templ.rows + 1;
        
        result.create( result_rows, result_cols, CV_32FC1 );
        
        /// Do the Matching and Normalize
        matchTemplate( img, templ, result, match_method );
        normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
        
        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;
        
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        
        
        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
        { matchLoc = minLoc; }
        else
        { matchLoc = maxLoc; }
        
        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(255), 2, 2, 0 );
        putText(img_display, to_string(i + 1), Point(matchLoc.x + templ.cols / 2, matchLoc.y + templ.rows / 2), FONT_HERSHEY_SIMPLEX, 1.0, Scalar::all(255), 2, CV_AA);
//        rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(255), 2, 8, 0 );
        
        //    cout << matchLoc.x << " " << matchLoc.y << " ";
        //    cout << templ.cols << " " << templ.rows;
        //    cout << endl;
    }
    
    imshow( image_window, img_display );
//    imshow( result_window, result );
    return;
}

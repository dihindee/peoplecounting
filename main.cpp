//#include "mainwindow.h"
//#include <QApplication>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/video.hpp>
#include <opencv4/opencv2/objdetect.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <cstdio>

#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

Ptr<BackgroundSubtractor> pMOG2;
const static int SENSIVITY_VALUE = 20;
const static int BLUR_SIZE = 10;

int oneObject[2] = {0,0};
Rect objBoundingRect = {0,0,0,0};

void processVideo(char *videoFilename);

void searchForMove(Mat thresholdImage, Mat &cameraFeed){
    bool objDetected = false;
    Mat tmp;
    thresholdImage.copyTo(tmp);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(tmp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    objDetected = contours.size() > 0;
    if(objDetected){
        vector<vector<Point>> largestContourVec;
        largestContourVec.push_back(contours.at(contours.size()-1));
        objBoundingRect = boundingRect(largestContourVec.at(0));
        int xpos = objBoundingRect.x + objBoundingRect.width/2;
        int ypos = objBoundingRect.y + objBoundingRect.height/2;
        oneObject[0] = xpos, oneObject[1] = ypos;
    }
    rectangle(cameraFeed, objBoundingRect,Scalar(255,0,0),2);
}

int main(int argc, char *argv[])
{
    setUseOptimized(true);
    setNumThreads(4);
    pMOG2 = createBackgroundSubtractorMOG2(500,200,false);
    bool objDetected = false;
    bool debugMode = false;
    bool trackingEnabled = false;
    bool pause = false;
    Mat frame1, frame2;
    Mat gray1,gray2;
    Mat thresholdImage;
    VideoCapture cap;
    while(1){
        cap.open("video.mp4");
        if(!cap.isOpened()){
            cerr << "ERR VIDEO" << endl;
            getchar();
            return EXIT_FAILURE;
        }
        while(cap.get(CAP_PROP_POS_FRAMES)< cap.get(CAP_PROP_FRAME_COUNT)-1){
            cap.read(frame1);
            cvtColor(frame1, gray1, COLOR_BGR2GRAY);
            pMOG2->apply(gray1,thresholdImage);
//            threshold(thresholdImage, thresholdImage, SENSIVITY_VALUE, 255, THRESH_BINARY);
            if(debugMode){
                imshow("Threshold Image", thresholdImage);
            }else{
//                destroyWindow("Threshold Image");
            }
            blur(thresholdImage,thresholdImage, Size(BLUR_SIZE,BLUR_SIZE));
            threshold(thresholdImage,thresholdImage,SENSIVITY_VALUE, 255, THRESH_BINARY);
            if(debugMode){
                imshow("Final threshold", thresholdImage);
            }else{
//                destroyWindow("Final threshold");
            }

            if(trackingEnabled){
                searchForMove(thresholdImage, frame1);
            }
            imshow("Frame", frame1);
            switch(waitKey(10)){
                case 27: // esc
                    return EXIT_SUCCESS;
                case 116: // t
                    trackingEnabled= !trackingEnabled;
                    if(!trackingEnabled) cout<<"Tracking disabled."<<endl;
                    else cout<<"Tracking enabled."<<endl;
                    break;
                case 100: // d
                    debugMode = !debugMode;
                    if(debugMode == false) cout<<"Debug mode disabled."<<endl;
                    else cout<<"Debug mode enabled."<<endl;
                    break;
                case 112: // p
                    pause = !pause;
                    if(pause){
                        cout<<"Code paused, press 'p' again to resume"<<endl;
                        while (pause){
                            //stay in this loop until
                            switch (waitKey()){
                                //a switch statement inside a switch statement? Mind blown.
                                case 112:
                                    //change pause back to false
                                    pause = false;
                                    cout<<"Code Resumed"<<endl;
                                    break;
                            }
                        }
                    }
            }
        }
        cap.release();
    }
}

//void processVideo(char *videoFilename){
//    VideoCapture capture(videoFilename);
//    if(!capture.isOpened()){
//        //error in opening the video input
//        cerr << "Unable to open video file: " << videoFilename << endl;
//        exit(EXIT_FAILURE);
//    }
//    keyboard = 0;
//    while( keyboard != 'q' && keyboard != 27){
//        if(!capture.read(frame)){
//            cerr << "Unable to read next frame." << endl;
//            cerr << "Exiting..." << endl;
//            exit(EXIT_FAILURE);
//        }
//        Mat grayFrame;
//        cvtColor(frame, grayFrame,COLOR_BGR2GRAY);
//        pMOG2->apply(grayFrame, fgMaskMOG2);
////        Mat threshed;
////        threshold(fgMaskMOG2,threshed,20,255,THRESH_BINARY);
//        stringstream strs;
//        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
//                  cv::Scalar(255,255,255), -1);
//        strs << capture.get(CAP_PROP_POS_FRAMES);
//        string frameNumberString = strs.str();
//        putText(frame, frameNumberString, cv::Point(15,15),
//                FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0));
//        imshow("Frame", frame);
//        imshow("FG Mask MOG 2", fgMaskMOG2);
////        imshow("Threshed", threshed);
//        keyboard = (char)waitKey(30);
//    }
//    capture.release();
//}


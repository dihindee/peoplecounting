#ifndef PEOPLETRACKING_H
#define PEOPLETRACKING_H
#include <QLabel>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/video.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <cstdio>
using namespace cv;
using namespace std;

class peopleTracking{
public:
    Ptr<BackgroundSubtractor> pMOG2;
    VideoCapture cap;
    Mat drawableFrame;
    int THRESHOLD_VALUE;
    int BLUR_SIZE;
    bool isDebugMode;
    bool isTrackingEnabled;
    bool isPaused;
    bool isProcessStarted;

    peopleTracking();
    void startProcessing(QLabel *labelToDraw);
    void searchForMove(Mat &thresholdImage, Mat &cameraFeed);
};

#endif // PEOPLETRACKING_H

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
    QPointF start,end;
    bool isInverted;
    int peoplePassed = 0;

    peopleTracking();
    void startProcessing(QWidget *widgetToDraw);
    void searchForMove(Mat &thresholdImage, Mat &cameraFeed);
    void setBorder(QPointF start, QPointF end, bool isInverted);
protected:
    void registerObject(Point2f pos);
    void deRegisterObject(int id);
    map<int, Point2f> objects;
    map<int, int> disappeared;
    map<int, bool> isPassed;
    int nextId;
};

#endif // PEOPLETRACKING_H

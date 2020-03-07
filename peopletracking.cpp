#include "peopletracking.h"
#include <unistd.h>
peopleTracking::peopleTracking(){
    this->pMOG2 = createBackgroundSubtractorMOG2(1000, 200, false);
    this->THRESHOLD_VALUE = 20;
    this->BLUR_SIZE = 20;
    this->isDebugMode = false;
    this->isTrackingEnabled = false;
    this->isPaused = false;
    this->isProcessStarted = false;
}
void peopleTracking::startProcessing(QLabel *labelToDraw){
    struct timespec pause_sleep_time = {0, 50000000};
    Mat frame, gray, thresholdImage, bluredImage;
    while(cap.get(CAP_PROP_POS_FRAMES)< cap.get(CAP_PROP_FRAME_COUNT)-1){
        if(!isPaused){
            cap.read(frame);
            resize(frame,frame,Size(480,270));
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            pMOG2->apply(gray, thresholdImage);
        }
        frame.copyTo(drawableFrame);
        blur(thresholdImage, bluredImage, Size(BLUR_SIZE, BLUR_SIZE));
        threshold(bluredImage, bluredImage, THRESHOLD_VALUE, 255, THRESH_BINARY);
        if(isDebugMode)
            imshow("final frame edit", bluredImage);
        if(isTrackingEnabled)
            searchForMove(bluredImage, drawableFrame);
        labelToDraw->setPixmap(QPixmap::fromImage(QImage(drawableFrame.data,
                                                         drawableFrame.cols,
                                                         drawableFrame.rows,
                                                         drawableFrame.step, QImage::Format_BGR888)));
        if(isPaused)
            nanosleep(&pause_sleep_time,&pause_sleep_time);
    }
    cap.release();
    labelToDraw->setText("end of video");
    isProcessStarted = false;
}
void peopleTracking::searchForMove(Mat &thresholdImage, Mat &cameraFeed){
    bool objDetected = false;
        Mat tmp;
        thresholdImage.copyTo(tmp);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(tmp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        objDetected = contours.size() > 0;
        if(objDetected){
            for(int i = 0; i < contours.size(); i++){
                Rect objBoundingRect = boundingRect(contours.at(i));
                if(objBoundingRect.width > 30 && objBoundingRect.height > 30)
                    rectangle(cameraFeed, objBoundingRect, Scalar(0,0,255),2);
            }
        }
}

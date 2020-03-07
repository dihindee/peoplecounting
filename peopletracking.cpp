#include "peopletracking.h"
#include <unistd.h>
peopleTracking::peopleTracking(){
    this->pMOG2 = createBackgroundSubtractorMOG2(1000, 200, false);
    this->THRESHOLD_VALUE = 20;
    this->BLUR_SIZE = 20;
    this->isDebugMode = false;
    this->isTrackingEnabled = true;
    this->isPaused = false;
    this->isProcessStarted = false;
}
void peopleTracking::startProcessing(QLabel *labelToDraw){
    struct timespec pause_sleep_time = {0, 10000000};
    Mat frame, gray, thresholdImage, bluredImage;
    while(cap.get(CAP_PROP_POS_FRAMES)< cap.get(CAP_PROP_FRAME_COUNT)-1){
//        nanosleep(&pause_sleep_time,&pause_sleep_time);
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
void peopleTracking::registerObject(Point2f pos){
    objects.insert(pair<int,Point2f>(nextId, pos));
    disappeared.insert(pair<int,int>(nextId,0));
    nextId++;
}
void peopleTracking::deRegisterObject(int id){
    objects.erase(id);
    disappeared.erase(id);
}

struct distEl{
    float dist;
    int id;
    Point2f centroid;
    bool isActive;
};
void peopleTracking::searchForMove(Mat &thresholdImage, Mat &cameraFeed){
    int maxDisappear = 10;
        Mat tmp;
        thresholdImage.copyTo(tmp);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(tmp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if(contours.empty()){
            for(auto &i : disappeared){
                i.second++;
                if(i.second > maxDisappear)
                    deRegisterObject(i.first);
            }
        }
        vector<Point2f> inputCentroids;
        for(int i = 0; i < contours.size(); i++){
            Rect bound = boundingRect(contours.at(i));
            if(bound.width > 10 && bound.height > 10){
                rectangle(cameraFeed, bound, Scalar(0,0,255),2);
                inputCentroids.push_back(Point2f(bound.x+bound.width/2,bound.y+bound.height/2));
            }
        }
        if(objects.empty()){
            for(auto &i : inputCentroids)
                registerObject(i);
        }
        else{
            int leftIds = objects.size(), leftCentroids = inputCentroids.size();
            vector<distEl> dists;
            for(auto &i : objects){
                for(auto &c: inputCentroids){
                    dists.push_back(distEl{abs(c.x*c.x-i.second.x*i.second.x) + abs(c.y*c.y - i.second.y*i.second.y),i.first, c, true});
                }
            }
            std::sort(dists.begin(),dists.end(),[](const distEl &a, const distEl &b)->bool{return a.dist>b.dist;});
            vector<distEl> paired;
            int i = 0;
            while(leftIds > 0 && leftCentroids > 0){
                while(!dists[i].isActive) i++;
                paired.push_back(dists[i]);
                leftIds--;
                leftCentroids--;
                if(leftIds>0 && leftCentroids>0){
                    for(int j=i; j<dists.size();j++){
                        if(dists[j].id == dists[i].id || dists[j].centroid == dists[i].centroid)
                            dists[j].isActive = false;
                    }
                }
                else{
                    if(leftIds==0 && leftCentroids==0) continue;
                    if(leftIds==0){
                        for(int j=i+1; j< dists.size();j++){
                            if(dists[j].isActive)
                                registerObject(dists[j].centroid);
                        }
                    }
                    if(leftCentroids==0){
                        for(int j=i+1; j< dists.size(); j++){
                            disappeared[dists[j].id]++;
                            if(disappeared[dists[j].id] > maxDisappear)
                                deRegisterObject(dists[j].id);
                        }
                    }
                }
            }
            for(int i=0;i<paired.size();i++){
                objects[paired[i].id] = paired[i].centroid;
                disappeared[paired[i].id] = 0;
            }
        }
        for(auto &i: objects){
            circle(cameraFeed,i.second,0,Scalar(0,255,0),5);
        }
}

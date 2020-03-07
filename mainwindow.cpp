#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QThread>
#include "peopletracking.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_video_triggered()
{
    QString name = QFileDialog::getOpenFileName(this,"Select video to open");
    if(!name.isEmpty()){
        if(tracker->cap.isOpened())
            tracker->cap.release();
        tracker->cap.open(name.toStdString());
        if(!tracker->cap.isOpened())
            cerr << "error opening video" << endl;
        if(!tracker->isProcessStarted){
            QThread *thread = QThread::create([this]{this->tracker->startProcessing(this->ui->img_label);});
//            tracker->startProcessing(this->ui->img_label);
            thread->start();
        }
    }
}

void MainWindow::on_thresholdSlider_valueChanged(int value)
{
    tracker->THRESHOLD_VALUE = value;
    cout << "new THRESHOLD_VALUE: " << value << endl;
}

void MainWindow::on_blurSlider_valueChanged(int value)
{
    tracker->BLUR_SIZE = value;
    cout << "new BLUR_SIZE: " << value << endl;
}


void MainWindow::on_debugBox_stateChanged(int arg1)
{
    tracker->isDebugMode = (arg1 !=0);
}

void MainWindow::on_trackingBox_stateChanged(int arg1)
{
    tracker->isTrackingEnabled  = (arg1!=0);
}

void MainWindow::on_playPauseButton_clicked()
{
    tracker->isPaused = !tracker->isPaused;
    this->ui->playPauseButton->setText(tracker->isPaused?"Play":"Pause");
}

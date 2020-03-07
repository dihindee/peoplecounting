#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "peopletracking.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    unique_ptr<peopleTracking> tracker;
    ~MainWindow();

private slots:
    void on_actionOpen_video_triggered();

    void on_thresholdSlider_valueChanged(int value);

    void on_blurSlider_valueChanged(int value);

    void on_debugBox_stateChanged(int arg1);

    void on_trackingBox_stateChanged(int arg1);

    void on_playPauseButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

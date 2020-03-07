#include "mainwindow.h"
#include "peopletracking.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    unique_ptr<peopleTracking> tracker(new  peopleTracking());
//    peopleTracking::init();
    w.tracker = std::move(tracker);
    w.show();
    return a.exec();
}

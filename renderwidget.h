#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QImage>
#include <QWidget>
#include <QGraphicsView>
#include "mainwindow.h"


class RenderWidget: public  QGraphicsView
{
    QImage frame;
    QPointF start,end;
    bool isPointsInverted = false, isMouseTracking = false;
    void drawBackground(QPainter *painter, const QRectF &rect);
public:
    QMainWindow *parentWindow;
    RenderWidget();
    void updateFrame(QImage newFrame);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // RENDERWIDGET_H

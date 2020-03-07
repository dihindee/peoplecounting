#include "renderwidget.h"
#include <QMouseEvent>

RenderWidget::RenderWidget()
{
    this->setScene(new QGraphicsScene);

}
void RenderWidget::updateFrame(QImage newFrame){
    this->frame = newFrame;
    this->scene()->update();
}

void RenderWidget::drawBackground(QPainter *painter, const QRectF &rect){
//    cout << "render" << endl;
    painter->resetTransform();
    painter->fillRect(0,0,this->width(),this->height(),QColor(0,0,0));
    painter->drawImage(QRectF(0,0,this->width(),this->height()),frame);
    painter->setPen(QColor(0,0,255));
    painter->drawLine(start, end);
    if((end.x()-start.x()!=0 ) && std::abs(end.y()-start.y())/(end.x()-start.x())<1){
        //vertical
        painter->drawText(QPointF(start.x(),start.y()+(isPointsInverted?-15:15)),"in");
        painter->drawText(QPointF(start.x(), start.y()-(isPointsInverted?-15:15)), "out");
    }else{
        //horizontal
        painter->drawText(QPointF(start.x()-(isPointsInverted?-15:15),start.y()),"in");
        painter->drawText(QPointF(start.x()+(isPointsInverted?-15:15), start.y()), "out");
    }
}

void RenderWidget::mousePressEvent(QMouseEvent *event){
    if(!((MainWindow*)(this->parentWindow))->tracker->isPaused)
        ((MainWindow*)(this->parentWindow))->togglePause();
    start.setX(event->x());
    start.setY(event->y());
    end.setX(event->x());
    end.setY(event->y());
    if(end.x() < start.x())
        isPointsInverted = true;
    else
        isPointsInverted = false;
    isMouseTracking = true;
    this->scene()->update();
}
void RenderWidget::mouseMoveEvent(QMouseEvent *event){
    if(isMouseTracking){
        end.setX(event->x());
        end.setY(event->y());
        if(end.x() < start.x())
            isPointsInverted = true;
        else
            isPointsInverted = false;
        this->scene()->update();
    }
}
void RenderWidget::mouseReleaseEvent(QMouseEvent *event){
    isMouseTracking = false;
    end.setX(event->x());
    end.setY(event->y());
    if(isPointsInverted){
        QPointF tmp = start;
        start = end;
        end = tmp;
    }
    ((MainWindow*)(this->parentWindow))->tracker->setBorder(start,end,isPointsInverted);
    this->scene()->update();
}

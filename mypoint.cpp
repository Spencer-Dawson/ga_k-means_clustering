#include "mypoint.h"
#include <cmath>

MyPoint::MyPoint()
{
    x=y=x2=y2=1;
    brush=Qt::black;
}

MyPoint::MyPoint(double inputX, double inputY, QColor input_color)
{
    setPoint(inputX, inputY, input_color);
}

void MyPoint::setPoint(double inputX, double inputY, QColor input_color)
{
    x=inputX;
    y=inputY;
    brush=input_color;

    if(fmod(x,1)>=.5)
        x2=static_cast<int>(x);
    else
        x2=static_cast<int>(x);

    if(fmod(y,1)>=.5)
        y2=static_cast<int>(y);
    else
        y2=static_cast<int>(y);

}

QRectF MyPoint::boundingRect() const
{
    return QRect(x2, y2, 3, 3);
}

void MyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

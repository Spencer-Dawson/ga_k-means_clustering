#ifndef MYPOINT_H
#define MYPOINT_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

class MyPoint : public QGraphicsItem
{
    public:
        MyPoint();
        MyPoint(double, double,  QColor);
        //~MyPoint();
        void setPoint(double, double, QColor);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    protected:
        double x, y;
        int x2, y2;
        QColor brush;

};

#endif // MYPOINT_H

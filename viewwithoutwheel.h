#ifndef VIEWWITHOUTWHEEL_H
#define VIEWWITHOUTWHEEL_H

#include <QGraphicsView>

class ViewWithoutWheel : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ViewWithoutWheel(QWidget *parent = 0) : QGraphicsView(parent)
    {
    }

public slots:
    virtual void wheelEvent(QWheelEvent *e) override
    {
    }
    virtual void resizeEvent(QResizeEvent *e) override
    {
        QGraphicsView::resizeEvent(e);
        if (scene() != nullptr)
            fitInView(scene()->items()[0], Qt::KeepAspectRatio);
    }
};

#endif
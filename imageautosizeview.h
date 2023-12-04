#ifndef IMAGEAUTOSIZEVIEW_H
#define IMAGEAUTOSIZEVIEW_H

#include <QPixmap>
#include <QResizeEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>

class ImageAutoSizeView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageAutoSizeView(QWidget *parent = 0) : QGraphicsView(parent)
    {
    }

public slots:
    void resizeEvent(QResizeEvent *e)
    {
        }
};

#endif
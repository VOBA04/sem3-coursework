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
    QGraphicsPixmapItem *gpixmap;
    QPixmap pixmap;

public:
    explicit ImageAutoSizeView(QWidget *parent = 0) : QGraphicsView(parent)
    {
        gpixmap = new QGraphicsPixmapItem;
    }
    void image_in_view(QPixmap &p)
    {
        pixmap = p;
        pixmap.scaled(QSize(size().width() - 10, size().height() - 10), Qt::KeepAspectRatio);
        gpixmap->setPixmap(pixmap);
        scene()->addItem(gpixmap);
    }
    ~ImageAutoSizeView()
    {
        delete gpixmap;
    }

public slots:
    void resizeEvent(QResizeEvent *e)
    {
        pixmap.scaled(QSize(e->size().width() - 10, e->size().height() - 10), Qt::KeepAspectRatio);
        gpixmap->setPixmap(pixmap);
        QGraphicsView::resizeEvent(e);
    }
};

#endif
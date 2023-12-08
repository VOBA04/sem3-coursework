#ifndef ICONAUTOSIZEPUSHBUTTON_H
#define ICONAUTOSIZEPUSHBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QResizeEvent>

class IconautosizePushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit IconautosizePushButton(QWidget *parent = 0) : QPushButton(parent)
    {
    }
    void set_image_path(QString &s)
    {
        image_path = s;
    }
    QString &get_image_path()
    {
        return image_path;
    }
public slots:
    void resizeEvent(QResizeEvent *e) override
    {
        setIconSize(icon().actualSize(QSize(
            e->size().width() - 10,
            e->size().height() - 10)));
        QPushButton::resizeEvent(e);
    }

private:
    QString image_path;
};

#endif // ICONAUTOSIZEPUSHBUTTON_H
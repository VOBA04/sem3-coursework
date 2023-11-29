#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include "mainwindow.h"
#include <QString>
#include <QFile>

void MainWindow::set_rec_opened_butts()
{
    QFile file;
    file.setFileName("D:\\University\\cs\\sem3\\cursach\\photored\\recently_opened.json");
    file.open(QIODevice::ReadOnly);
    QString s;
    s = file.readAll();
    file.close();
    QJsonDocument d;
    d = QJsonDocument::fromJson(s.toUtf8());
    QJsonArray pathes = d.array();
    QJsonValue path;
    int n = pathes.size();
    for (int i = 0; i < n; i++)
    {
        path = pathes[i];
        s = path.toString();
        QPixmap im(s);
        QIcon but_im(im);
        switch (i)
        {
        case 0:
            pushButton_Rec_open_1->set_image_path(s);
            pushButton_Rec_open_1->setIcon(but_im);
            pushButton_Rec_open_1->show();
            break;
        case 1:
            pushButton_Rec_open_2->set_image_path(s);
            pushButton_Rec_open_2->setIcon(but_im);
            pushButton_Rec_open_2->show();
            break;
        case 2:
            pushButton_Rec_open_3->set_image_path(s);
            pushButton_Rec_open_3->setIcon(but_im);
            pushButton_Rec_open_3->show();
            break;
        case 3:
            pushButton_Rec_open_4->set_image_path(s);
            pushButton_Rec_open_4->setIcon(but_im);
            pushButton_Rec_open_4->show();
            break;
        case 4:
            pushButton_Rec_open_5->set_image_path(s);
            pushButton_Rec_open_5->setIcon(but_im);
            pushButton_Rec_open_5->show();
            break;
        }
    }
}

void MainWindow::set_curr_proc(PROCESSES n)
{
    current_process = n;
}

PROCESSES MainWindow::get_curr_proc()
{
    return current_process;
}

void MainWindow::set_slider_limits()
{
    PROCESSES n = get_curr_proc();
    out_amount->setNum(0);
    regulation->setMinimum(-100);
    regulation->setMaximum(100);
    switch (n)
    {
    case PROCESSES::BRIGHTNESS:
    {
        regulation->setValue(image_info.brightness);
        break;
    }
    case PROCESSES::CONTRAST:
    {
        regulation->setValue(image_info.contrast);
        break;
    }
    case PROCESSES::SATURATUIN:
    {
        regulation->setValue(image_info.saturation);
        break;
    }
    case PROCESSES::CLARITY:
    {
        regulation->setValue(image_info.clarity);
        break;
    }
    case PROCESSES::TEMPERATURE:
    {
        regulation->setValue(image_info.temperture);
        break;
    }
    }
}
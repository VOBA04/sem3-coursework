#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include "mainwindow.h"
#include <QString>
#include <QFile>

void set_rec_opened_butts(MainWindow *w)
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
            w->pushButton_Rec_open_1->set_image_path(s);
            w->pushButton_Rec_open_1->setIcon(but_im);
            w->pushButton_Rec_open_1->show();
            break;
        case 1:
            w->pushButton_Rec_open_2->set_image_path(s);
            w->pushButton_Rec_open_2->setIcon(but_im);
            w->pushButton_Rec_open_2->show();
            break;
        case 2:
            w->pushButton_Rec_open_3->set_image_path(s);
            w->pushButton_Rec_open_3->setIcon(but_im);
            w->pushButton_Rec_open_3->show();
            break;
        case 3:
            w->pushButton_Rec_open_4->set_image_path(s);
            w->pushButton_Rec_open_4->setIcon(but_im);
            w->pushButton_Rec_open_4->show();
            break;
        case 4:
            w->pushButton_Rec_open_5->set_image_path(s);
            w->pushButton_Rec_open_5->setIcon(but_im);
            w->pushButton_Rec_open_5->show();
            break;
        }
    }
}
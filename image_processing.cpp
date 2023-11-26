#include "mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace fs = std::filesystem;

void start_proc(MainWindow *w, QString &QPath)
{
    QString Qpath_from;
    if (QPath.isEmpty())
    {
        Qpath_from = QFileDialog::getOpenFileName(w, QObject::tr("Open file"), "D:\\", QObject::tr("Images (*.png *.jpg)"), nullptr, QFileDialog::DontUseNativeDialog);
        for (int i = 0; i < Qpath_from.size(); i++)
        {
            if (Qpath_from[i] == '/')
                Qpath_from[i] = '\\';
        }
    }
    else
        Qpath_from = QPath;
    QString Qpath_to, Qfile_name;
    for (auto it = Qpath_from.rbegin(); *it != '\\'; it++)
        Qfile_name.push_back(*it);
    std::reverse(Qfile_name.begin(), Qfile_name.end());
    Qpath_to = "image_in_processing\\" + Qfile_name;
    fs::create_directory("image_in_processing");
    QFile::remove(Qpath_to);
    QFile image(Qpath_from);
    image.copy(Qpath_to);
    QPixmap Qimage(Qpath_to);
    w->label_greeting->hide();
    w->label_image->setPixmap(Qimage);
    w->label_image->show();
    w->pushButton_brightness->show();
    w->pushButton_clarity->show();
    w->pushButton_colorfulness->show();
    w->pushButton_contrast->show();
    w->pushButton_crop->show();
    w->pushButton_saturation->show();
    QFile file;
    file.setFileName("D:\\University\\cs\\sem3\\cursach\\photored\\recently_opened.json");
    file.open(QIODevice::ReadWrite);
    QString s;
    s = file.readAll();
    QJsonDocument d;
    d = QJsonDocument::fromJson(s.toUtf8());
    QJsonArray pathes = d.array();
    QJsonValue path(Qpath_from);
    if (!pathes.contains(path))
    {
        int n = pathes.size();
        if (n < 5)
        {
            pathes.push_back(path);
        }
        else
        {
            pathes.pop_front();
            pathes.push_back(path);
        }
        d.setArray(pathes);
        s = d.toJson();
        file.resize(0);
        file.write(s.toUtf8());
    }
    file.close();
}

void image_brightness()
{
}
#include "mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

namespace fs = std::filesystem;

void start_proc(MainWindow *w, QString &QPath)
{
    QString Qpath;
    if (QPath.isEmpty())
    {
        Qpath = QFileDialog::getOpenFileName(w, QObject::tr("Open file"), "D:\\", QObject::tr("Images (*.png *.jpg)"), nullptr, QFileDialog::DontUseNativeDialog);
        for (int i = 0; i < Qpath.size(); i++)
        {
            if (Qpath[i] == '/')
                Qpath[i] = '\\';
        }
    }
    else
        Qpath = QPath;
    std::string path_from = Qpath.toStdString(), path_to;
    std::string file_name;
    for (auto it = path_from.rbegin(); *it != '\\'; it++)
        file_name.push_back(*it);
    std::reverse(file_name.begin(), file_name.end());
    path_to = "image_in_processing\\" + file_name;
    fs::create_directory("image_in_processing");
    fs::remove(path_to);
    fs::copy_file(path_from, path_to);
    QString Qpath_from = Qpath;
    Qpath.clear();
    for (auto it : path_to)
        Qpath.push_back(it);
    QPixmap qimage(Qpath);
    w->label_greeting->hide();
    w->label_image->setPixmap(qimage);
    w->label_image->show();
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
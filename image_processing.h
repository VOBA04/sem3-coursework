#include "mainwindow.h"
#include <iostream>

namespace fs = std::filesystem;

void start_proc(MainWindow *w)
{
    QString Qpath = QFileDialog::getOpenFileName(w, QObject::tr("Open file"), "D:\\", QObject::tr("Images (*.png *.jpg)"), nullptr, QFileDialog::DontUseNativeDialog);
    std::cout << Qpath.toStdString();
    for (int i = 0; i < Qpath.size(); i++)
    {
        if (Qpath[i] == '/')
            Qpath[i] = '\\';
    }
    // QString Qpath = "D:\\University\\cs\\sem3\\cursach\\test.png";
    std::string path_from = Qpath.toStdString(), path_to;
    std::string file_name;
    for (auto it = path_from.rbegin(); *it != '\\'; it++)
        file_name.push_back(*it);
    std::reverse(file_name.begin(), file_name.end());
    path_to = "image_in_proccesing\\" + file_name;
    fs::create_directory("image_in_proccesing");
    fs::remove(path_to);
    fs::copy_file(path_from, path_to);
    Qpath.clear();
    for (auto it : path_to)
        Qpath.push_back(it);
    QPixmap qimage(Qpath);
    // w->label_image->setScaledContents(true);
    // w->label_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    w->label_image->setPixmap(qimage);
    w->label_image->show();
}
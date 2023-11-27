#include "mainwindow.h"

#include <QApplication>
#include <cstdlib>
#include <iostream>

#include "image_processing.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

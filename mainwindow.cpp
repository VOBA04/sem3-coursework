#include "mainwindow.h"
#include "image_processing.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    label_image->hide();
    QObject::connect(pushButton_New, &QPushButton::clicked, this, [&]()
                     { start_proc(this); });
}

MainWindow::~MainWindow()
{
}

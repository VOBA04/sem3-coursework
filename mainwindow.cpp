#include "mainwindow.h"
#include "image_processing.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    label_image->hide();
    pushButton_Rec_open_1->hide();
    pushButton_Rec_open_2->hide();
    pushButton_Rec_open_3->hide();
    pushButton_Rec_open_4->hide();
    pushButton_Rec_open_5->hide();
    set_rec_opened_butts(this);
    QObject::connect(pushButton_New, &QPushButton::clicked, this, [&]()
                     { QString tmp; start_proc(this, tmp); });
    QObject::connect(pushButton_Rec_open_1, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_1->get_image_path()); });
    QObject::connect(pushButton_Rec_open_2, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_2->get_image_path()); });
    QObject::connect(pushButton_Rec_open_3, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_3->get_image_path()); });
    QObject::connect(pushButton_Rec_open_4, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_4->get_image_path()); });
    QObject::connect(pushButton_Rec_open_5, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_5->get_image_path()); });
}

MainWindow::~MainWindow()
{
}

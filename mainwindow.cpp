#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cv::Mat image = cv::imread("D:\\University\\cs\\sem3\\photored\\test.png");
    cv::namedWindow("My Image");
    cv::imshow("My Image", image);
    // cv::waitKey(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTranslator>
#include "ui_mainwindow.h"
#include <filesystem>
#include <string>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "workerthread.h"
#include "cvmatandqimage.h"
#include <opencv2/core/core.hpp>
#include <QStandardPaths>

enum class PROCESSES
{
    BRIGHTNESS,
    CONTRAST,
    SATURATUIN,
    CLARITY,
    TEMPERATURE,
    CROP
};

namespace fs = std::filesystem;

class MyThread;

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT
    MyThread *mythread;
    PROCESSES current_process;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void set_curr_proc(PROCESSES);
    void set_rec_opened_butts();
    PROCESSES get_curr_proc();
    ~MainWindow();

public slots:
    void start_proc(QString &);
    void main_proc(int);
    void set_slider_limits();
    void end_main_proc();
    void rotate_left();
    void rotate_right();
    void save_image();
    void set_new_image();

protected:
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *pixmap;
    struct image_info
    {
        QPixmap *start_image;
        int brightness, contrast, saturation, clarity, temperture;
    } image_info;

public slots:
    void change_image(cv::Mat);
};

#endif // MAINWINDOW_H

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

enum class PROCESSES
{
    BRIGHTNESS,
    CONTRAST,
    SATURATUIN,
    CLARITY,
    COLORFULNESS,
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
    friend void start_proc(MainWindow *, QString &);
    friend void main_proc(MainWindow *);
    friend void end_main_proc(MainWindow *);
    friend void set_rec_opened_butts(MainWindow *);
    friend void set_slider_limits(MainWindow *);
    void set_curr_proc(PROCESSES);
    PROCESSES get_curr_proc();
    ~MainWindow();

protected:
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *pixmap;
    struct image_info
    {
        QPixmap *start_image;
        int brightness, contrast, saturation, clarity, colorfulness;
    } image_info;

public slots:
    void change_image(cv::Mat);
};

#endif // MAINWINDOW_H

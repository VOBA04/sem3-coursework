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
#include "MyRing.h"
#include "image_filters.h"

enum class PROCESSES
{
    BRIGHTNESS,
    CONTRAST,
    SATURATUIN,
    CLARITY,
    TEMPERATURE,
    CROP,
    ROTATION,
    FILTER,
    NON
};

enum class FILTER
{
    INVERSE,
    ORIGINAL,
    GRAY,
    CUSTOM
};

namespace fs = std::filesystem;

class MyThread;
class Filter;

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT
    MyThread *mythread;
    PROCESSES current_process;
    MyRing<Filter *> filters;
    int filter_number;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void set_curr_proc(PROCESSES);
    void set_rec_opened_butts();
    PROCESSES get_curr_proc();
    void prepare_image();
    void set_filters();
    void save_filters();
    cv::Mat get_filtered_image(int);
    std::string get_filter_name(int);
    void set_filter_number(int);
    void next_prev_filter(int);
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
    void set_filters_buttons();
    void set_deleteF_enabled(std::string);
    void back_from_filters();
    void apply_filter();
    void delete_filter();

protected:
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *pixmap;
    struct image_info
    {
        QPixmap *start_image;
        QPixmap *image_in_proc;
        int brightness, contrast, saturation, clarity, temperture;
        FILTER filter;
    } image_info;

public slots:
    void change_image(cv::Mat);
};

#endif // MAINWINDOW_H

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
#include "workerthread.h"

namespace fs = std::filesystem;

class MyThread;

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT
    MyThread *mythread;
    int current_process;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    friend void start_proc(MainWindow *, QString &);
    friend void main_proc(MainWindow *);
    friend void set_rec_opened_butts(MainWindow *);
    friend void set_slider_limits(MainWindow *);
    void set_curr_proc(int);
    int get_curr_proc();
    ~MainWindow();

public slots:
    void change_image();
};

#endif // MAINWINDOW_H

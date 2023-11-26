#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTranslator>
#include "ui_mainwindow.h"
#include <filesystem>
#include <string>

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    friend void start_proc(MainWindow *, QString &);
    friend void set_rec_opened_butts(MainWindow *);
    ~MainWindow();
};

#endif // MAINWINDOW_H

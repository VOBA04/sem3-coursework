#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <queue>
#include "mainwindow.h"
#include "image_processing.h"
#include <opencv2/core/core.hpp>

class MainWindow;
class Operation;

class MyThread : public QThread
{
    Q_OBJECT
public:
    std::queue<Operation *> queue;
    MyThread() {}
    void push(Operation *);

protected:
    virtual void run() override;
signals:
    void signalGUI(cv::Mat);
public slots:
    void terminateThread()
    {
        if (isRunning())
        {
            requestInterruption();
            wait();
        }
    }
};

#endif
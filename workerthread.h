#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <queue>
#include "mainwindow.h"
#include "image_processing.h"

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
    virtual void run();
signals:
    void signalGUI();
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
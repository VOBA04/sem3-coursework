#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include "image_processing.h"
#include "mainwindow.h"
#include <QThread>
#include <opencv2/core/core.hpp>
#include <queue>

class MainWindow;
class Operation;

class MyThread : public QThread {
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
  void terminateThread() {
    if (isRunning()) {
      requestInterruption();
      wait();
    }
  }
};

#endif
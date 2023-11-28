#include "workerthread.h"

void MyThread::run()
{
    while (!isInterruptionRequested())
    {
        while (queue.empty())
        {
            if (isInterruptionRequested())
                return;
            msleep(10);
        }
        Operation *oper_now = queue.front();
        queue.pop();
        cv::Mat image = oper_now->exec();
        emit signalGUI(image);
    }
}

void MyThread::push(Operation *oper)
{
    queue.push(oper);
}
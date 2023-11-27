#include "workerthread.h"

void MyThread::run()
{
    while (!isInterruptionRequested())
    {
        while (queue.empty())
        {
            if (isInterruptionRequested())
                return;
            msleep(100);
        }
        Operation *oper_now = queue.front();
        queue.pop();
        oper_now->exec();
        emit signalGUI();
    }
}

void MyThread::push(Operation *oper)
{
    queue.push(oper);
}
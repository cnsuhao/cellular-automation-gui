#include <QDebug>
#include "calthread.h"
#include "CellMatrix.h"
#include "mainwindow.h"

CalThread::CalThread(int st, int ed, MainWindow *mw, QObject *parent) :
     QThread(parent), startid(st), endid(ed), mainwindow(mw), myStepCounter(0)
{
}

void CalThread::run()
{
    while (true)
    {
        // wait for resume signal
        mainwindow->stepMutex.lockForRead();
        while (mainwindow->stepCounter != myStepCounter + 1)
        {
            qDebug() << "thread pause, my counter:" << myStepCounter << "main counter: " << mainwindow->stepCounter;
            mainwindow->stepCond.wait(&mainwindow->stepMutex);
        }
        mainwindow->stepMutex.unlock();

        qDebug() << "thread resume";
        // done the job
        int i;
        for (i = startid; i < endid; i++)
            mainwindow->matrix->step(i);

        // report complete
        emit workDone();

        // update my counter
        myStepCounter++;
    }
}

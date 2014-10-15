#include <QDebug>
#include "calthread.h"
#include "CellMatrix.h"

CalThread::CalThread(CellMatrix *mat, int st, int ed, QObject *parent) :
    matrix(mat), startid(st), endid(ed), resumed(true), QThread(parent)
{
}

void CalThread::run()
{
    while (true)
    {
        // wait for resume signal
        resumedMutex.lock();
        while (!resumed)
        {
            qDebug() << "thread pause";
            resumedCond.wait(&resumedMutex);
        }
        resumed = false;
        resumedMutex.unlock();

        qDebug() << "thread resume";
        // done the job
        int i;
        for (i = startid; i < endid; i++)
            matrix->step(i);

        // report complete
        emit workDone();
    }
}

void CalThread::resume()
{
    resumedMutex.lock();
    resumed = true;
    resumedCond.wakeAll();
    resumedMutex.unlock();
}

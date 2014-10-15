#ifndef CALTHREAD_H
#define CALTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class CellMatrix;
class MainWindow;

class CalThread : public QThread
{
    Q_OBJECT
public:
    explicit CalThread(CellMatrix *mat, int startid, int endid, QObject *parent);

    void resume();

signals:
    void workDone();

public slots:

private:
    void run();

    CellMatrix *matrix;
    int startid;
    int endid;

    bool resumed;
    QMutex resumedMutex;
    QWaitCondition resumedCond;
};

#endif // CALTHREAD_H

#ifndef CALTHREAD_H
#define CALTHREAD_H

#include <QThread>

class MainWindow;

class CalThread : public QThread
{
    Q_OBJECT
public:
    explicit CalThread(int startid, int endid, MainWindow *mainwindow, QObject *parent);

signals:
    void workDone();

public slots:

private:
    void run();

    int startid;
    int endid;
    MainWindow *mainwindow;

    unsigned long myStepCounter;
};

#endif // CALTHREAD_H

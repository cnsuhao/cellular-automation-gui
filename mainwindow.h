#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QReadWriteLock>
#include <QWaitCondition>
#include "calthread.h"

class Button;
class CellMatrix;
class QStatusBar;
class CalThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int w, int h, QWidget *parent = 0);
    ~MainWindow();

    friend class CalThread;

    void start();

public slots:
    void threadWorkDone();

private:
    unsigned int width;
    unsigned int height;
    QList<Button *> buttons;
    CellMatrix *matrix;
    QStatusBar *statusBar;
    QList<CalThread *> threads;
    unsigned long threadCounter;

    unsigned int stepCounter;
    QReadWriteLock stepMutex;
    QWaitCondition stepCond;

    void drawButtons();
    unsigned long getCurrentTime();
};

#endif // WIDGET_H

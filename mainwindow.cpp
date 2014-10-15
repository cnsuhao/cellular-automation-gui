#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QDebug>
#include <QStatusBar>
#include "mainwindow.h"
#include "CellMatrix.h"
#include "config.h"
#include "calthread.h"

MainWindow::MainWindow(int w, int h, QWidget *parent)
    : width(w), height(h), matrix(NULL), statusBar(NULL), threadCounter(0), QMainWindow(parent)
{
    matrix = new CellMatrix(width, height);
    matrix->setCellsExploreMode(true);      // start in explore mode

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setSizePolicy(sizePolicy);
    this->setWindowTitle("CA - Explore");

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    int i;
    QSizePolicy buttonSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    for (i = 0; i < width * height; i++)
    {
        QPushButton *button = new QPushButton(this);
        button->setSizePolicy(buttonSizePolicy);
        buttons.append(button);
    }

    QGridLayout *layout = new QGridLayout(centralWidget);
    int j;
    for (i = 0; i < height; i++ )
    {
        for (j = 0; j < width; j++)
        {
            layout->addWidget(buttons[(i * width) + j], i, j);
        }
    }

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);

    // create threads
    int cellsNum = width * height;
    if (cellsNum < g_threads)
        g_threads = cellsNum;
    int cellsPerThread = cellsNum / g_threads;
    int start = 0;
    for (i = 0; i < g_threads - 1; i++)
    {
        qDebug() << "thread " << i << "start: " << start << "end: " << start + cellsPerThread;
        CalThread *thread = new CalThread(matrix, start, start + cellsPerThread, this);
        connect(thread, SIGNAL(workDone()), this, SLOT(threadWorkDone()));
        threads.append(thread);
        start += cellsPerThread;
    }
    // give the remaining cells to the last one
    qDebug() << "thread " << i << "start: " << start << "end: " << cellsNum;
    CalThread *thread = new CalThread(matrix, start, cellsNum, this);
    connect(thread, SIGNAL(workDone()), this, SLOT(threadWorkDone()));
    threads.append(thread);

    // start all threads
    for (i = 0; i < g_threads; i++)
        threads[i]->start();
}

MainWindow::~MainWindow()
{
    delete matrix;

    QList<CalThread *>::iterator tit;
    for (tit = threads.begin(); tit != threads.end(); ++ tit)
    {
        (*tit)->terminate();
        (*tit)->wait(1000);
    }
}

void MainWindow::refreshColor()
{
    qDebug() << "------------------------- refresh color";
    static unsigned long count = 0;
    if (count++ == g_exploreCount)
    {
        matrix->setCellsExploreMode(false);
        this->setWindowTitle("CA - Online");
    }

    // update color
    matrix->updateColors();
    // draw cells
    int i;
    QPushButton *button;
    for (i = 0; i < width * height; i++)
    {
        int color = matrix->getCellColor(i);
        button = buttons[i];
        if (color == 0)
            button->setStyleSheet("background-color: black;");
        else
            button->setStyleSheet("background-color: white;");
    }

    statusBar->showMessage(QString("step: %1").arg(count));
}

void MainWindow::threadWorkDone()
{
    if (++threadCounter == threads.size())        // calculation is done
    {
        threadCounter = 0;
        refreshColor();

        // resume all threads to calculate the next
        QList<CalThread *>::iterator tit;
        for (tit = threads.begin(); tit != threads.end(); ++ tit)
        {
            (*tit)->resume();
        }
    }
}

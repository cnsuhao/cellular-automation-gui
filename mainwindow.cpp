#include <QWidget>
#include <QGridLayout>
#include <QKeyEvent>
#include <QDebug>
#include <QStatusBar>
#include <sys/timeb.h>
#include "mainwindow.h"
#include "CellMatrix.h"
#include "config.h"
#include "calthread.h"
#include "button.h"

MainWindow::MainWindow(int w, int h, QWidget *parent)
    : QMainWindow(parent), width(w), height(h), matrix(NULL), statusBar(NULL), threadCounter(0), stepCounter(0), cur_lmode(0), next_lmode(0)
{
    matrix = new CellMatrix(width, height);
    matrix->setCellsExploreMode(true);      // start in explore mode

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setSizePolicy(sizePolicy);
    this->setWindowTitle("CA - Explore");

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    unsigned int i;
    QSizePolicy buttonSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    for (i = 0; i < width * height; i++)
    {
        Button *button = new Button(centralWidget);
        button->setSizePolicy(buttonSizePolicy);
        buttons.append(button);
    }

    QGridLayout *layout = new QGridLayout(centralWidget);
    unsigned int j;
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
    unsigned int cellsNum = width * height;
    if (cellsNum < g_threads)
        g_threads = cellsNum;
    int cellsPerThread = cellsNum / g_threads;
    int start = 0;
    for (i = 0; i < g_threads - 1; i++)
    {
        qDebug() << "thread " << i << "start: " << start << "end: " << start + cellsPerThread;
        CalThread *thread = new CalThread(start, start + cellsPerThread, this, this);
        connect(thread, SIGNAL(workDone()), this, SLOT(threadWorkDone()));
        threads.append(thread);
        start += cellsPerThread;
    }
    // give the remaining cells to the last one
    qDebug() << "thread " << i << "start: " << start << "end: " << cellsNum;
    CalThread *thread = new CalThread(start, cellsNum, this, this);
    connect(thread, SIGNAL(workDone()), this, SLOT(threadWorkDone()));
    threads.append(thread);

    // start all threads
    stepCounter = 1;
    for (i = 0; i < g_threads; i++)
        threads[i]->start();
}

MainWindow::~MainWindow()
{
    delete matrix;
}

void MainWindow::drawButtons()
{
    unsigned int i;
    Button *button;
    for (i = 0; i < width * height; i++)
    {
        button = buttons[i];
        if (button->isInputColor())     // set cell color to button if input
            matrix->setCellColor(i, button->getColor());
        else    // set button color to cell
            button->setColor(matrix->getCellColor(i));
    }

    statusBar->showMessage(QString("step: %1").arg(stepCounter));
    this->repaint();
}

void MainWindow::threadWorkDone()
{
    if (++threadCounter == threads.size())        // one step calculation is done
    {
        threadCounter = 0;

        matrix->updateColors(); // update colors only, draw later

        // resume threads to calculate the next, no need to wait for the draw
        // switch to Online mode the counter reaches the threshold
        if (stepCounter == g_exploreCount)
        {
            matrix->setCellsExploreMode(false);
            this->setWindowTitle("CA - Online");
            cur_lmode = next_lmode = 1;
        }

        if (next_lmode != cur_lmode)
        {
            cur_lmode = next_lmode;
            if (cur_lmode == 0)
            {
                matrix->setCellsExploreMode(true);
                this->setWindowTitle("CA - Explore");
            }
            else
            {
                matrix->setCellsExploreMode(false);
                this->setWindowTitle("CA - Online");
            }
        }

        stepMutex.lockForWrite();
        stepCounter++;
        stepCond.wakeAll();
        stepMutex.unlock();

        // draw gui after all threads started
        drawButtons();
    }
}

unsigned long MainWindow::getCurrentTime()
{
    struct timeb tb;
    ftime(&tb);
    return 1000 * tb.time + tb.millitm;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    // terminate & delete all threads
    QList<CalThread *>::iterator tit;
    for (tit = threads.begin(); tit != threads.end(); ++ tit)
    {
        (*tit)->quit();
        (*tit)->wait(50);
        delete (*tit);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_E)
        next_lmode = 0;
    else if (event->key() == Qt::Key_O)
        next_lmode = 1;
}

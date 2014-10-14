#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>
#include <QStatusBar>
#include "mainwindow.h"
#include "CellMatrix.h"
#include "config.h"

MainWindow::MainWindow(int w, int h, QWidget *parent)
    : width(w), height(h), matrix(NULL), QMainWindow(parent)
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
    this->setLayout(layout);

    statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateColor()));
    timer->start(0);
}

MainWindow::~MainWindow()
{
    delete matrix;
    delete timer;
}

void MainWindow::updateColor()
{
    static unsigned long count = 0;
    if (count++ == g_exploreCount)
    {
        matrix->setCellsExploreMode(false);
        this->setWindowTitle("CA - Online");
    }

    matrix->step();
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_P)
        timer->isActive() ? timer->stop() : timer->start(3);
}

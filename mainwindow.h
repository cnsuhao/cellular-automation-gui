#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>

class QPushButton;
class CellMatrix;
class QStatusBar;
class CalThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int w, int h, QWidget *parent = 0);
    ~MainWindow();

    void start();

public slots:
    void threadWorkDone();

private slots:
    void refreshColor();

private:
    int width;
    int height;
    QList<QPushButton *> buttons;
    CellMatrix *matrix;
    QStatusBar *statusBar;
    QList<CalThread *> threads;
    unsigned long threadCounter;
};

#endif // WIDGET_H

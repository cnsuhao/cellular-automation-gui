#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>

class QPushButton;
class CellMatrix;
class QStatusBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int w, int h, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateColor();

private:
    int width;
    int height;
    QList<QPushButton *> buttons;
    CellMatrix *matrix;
    QTimer *timer;
    QStatusBar *statusBar;

    void keyPressEvent(QKeyEvent *event);
};

#endif // WIDGET_H

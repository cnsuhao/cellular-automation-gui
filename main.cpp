#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "config.h"

int g_exploreCount, g_width, g_height, g_threads;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("./ca.conf", QSettings::IniFormat);
    QString key;
    key = "ExploreCount";
    g_exploreCount = settings.value(key, 10000).toInt();
    key = "Width";
    g_width = settings.value(key, 5).toInt();
    key = "Height";
    g_height = settings.value(key, 5).toInt();
    key = "Threads";
    g_threads = settings.value(key, 1).toInt();
    if (g_threads <= 0)
        g_threads = 1;

    MainWindow w(g_width, g_height);
    w.show();

    return a.exec();
}

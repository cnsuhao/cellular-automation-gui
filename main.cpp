#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "config.h"

unsigned int g_exploreCount, g_width, g_height, g_threads;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("./ca.conf", QSettings::IniFormat);
    QString key;
    key = "ExploreCount";
    g_exploreCount = settings.value(key, 10000).toUInt();
    key = "Width";
    g_width = settings.value(key, 5).toUInt();
    key = "Height";
    g_height = settings.value(key, 5).toUInt();
    key = "Threads";
    g_threads = settings.value(key, 1).toUInt();

    MainWindow w(g_width, g_height);
    w.show();

    return a.exec();
}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //avdevice_register_all();
    //av_register_all();
    avformat_network_init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

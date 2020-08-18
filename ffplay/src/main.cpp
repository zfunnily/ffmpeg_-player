#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    av_register_all();
    avformat_network_init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

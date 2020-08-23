#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "avmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow :  public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //拉流的槽
    void slot_setVideoImage(QImage img) ;
    //摄像头的槽
    void slot_setCameraImage(QImage img);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    AVManager* av_manager_;


};
#endif // MAINWINDOW_H

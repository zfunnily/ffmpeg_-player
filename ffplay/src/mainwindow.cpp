#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    av_manager_ = new AVManager();

    // 拉流视频显示的信号
    QObject::connect(av_manager_, SIGNAL(sign_setVideoImage(QImage)), this, SLOT(slot_setVideoImage(QImage)));
    // 摄像头显示的信号
    QObject::connect(av_manager_, SIGNAL(sign_setCameraImage(QImage)), this, SLOT(slot_setCameraImage(QImage)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete av_manager_;
    av_manager_ = nullptr;
}

void MainWindow::slot_setCameraImage(QImage img)
{
    ui->device->setPixmap(QPixmap::fromImage(img)); // 在label上播放视频图片

    //延时
    QTime dieTime = QTime::currentTime().addMSecs(40);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::slot_setVideoImage(QImage img)
{
   ui->label->setPixmap(QPixmap::fromImage(img)); // 在label上播放视频图片

   //延时
   QTime dieTime = QTime::currentTime().addMSecs(40);
   while( QTime::currentTime() < dieTime )
       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::on_pushButton_clicked()
{
   QString strTxtEdt = ui->lineEdit->text();
   if (strTxtEdt.length() < 1)
   {
       qDebug() << "please input stream url";
       return;
   }
   av_manager_->startRtmp(strTxtEdt);
}

void MainWindow::on_pushButton_2_clicked()
{
    av_manager_->stopRtmp();
}

void MainWindow::on_pushButton_3_clicked()
{
    av_manager_->startCamera();
}

void MainWindow::on_pushButton_4_clicked()
{
    av_manager_->pushCamera();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    video_ = new VideoContext(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setVideoImage(QImage img)
{
   ui->label->setPixmap(QPixmap::fromImage(img)); // 在label上播放视频图片

   QTime dieTime = QTime::currentTime().addMSecs(40);
   while( QTime::currentTime() < dieTime )
       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::on_pushButton_clicked()
{
    QString strTxtEdt = ui->lineEdit->text();
if (strTxtEdt.length() < 1)
{
    qDebug() << "please input stream url！！！";
    return;
}
       video_->init(strTxtEdt);
       video_->findStreamIndex();
       video_->openCodeCtx();
       video_->setImage();
}

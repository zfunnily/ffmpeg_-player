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
    video_->releaseAV();

   QString strTxtEdt = ui->lineEdit->text();
   if (strTxtEdt.length() < 1)
   {
       qDebug() << "please input stream url";
       return;


   }
    AVFormatContext* format_ctx = avformat_alloc_context();
   // 打开音视频流
   if(avformat_open_input(&format_ctx,strTxtEdt.toStdString().c_str(),nullptr,nullptr)!=0){
       qDebug() << ("Couldn't open input stream.\n");
       return;
   }

   // 获取音视频流数据信息
   if(avformat_find_stream_info(format_ctx,nullptr)<0){
       qDebug() << ("Couldn't find stream information.\n");
       return ;
   }

   video_->init(format_ctx);
   video_->setImage(format_ctx);
}

void MainWindow::on_pushButton_2_clicked()
{
    video_->releaseAV();
}

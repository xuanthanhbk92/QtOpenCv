#include "videothread.h"
#include <QDebug>
#include "ui_mainwindow.h"
using namespace cv;

bool VideoThread::isProcess = false;

VideoThread::VideoThread(Ui::MainWindow *main, QObject *parent) : QThread(parent),ui_main(main)
{
}

void VideoThread::exitVideo()
{
    stop=true;
}

void VideoThread::run()
{
    qDebug() << "Video thread running ";
    if (ui_main->ldtVideoPath->text().isEmpty())
        return;
    VideoCapture cap(ui_main->ldtVideoPath->text().toStdString());
    if(!cap.isOpened()) {
        qDebug() << "Error opening video stream or file";
        return ;
    }
    if (isProcess == true)
        return;
    isProcess = true;
    while(1){
      Mat frame,m_srcGray,m_edge,m_dst;
      // Capture frame-by-frame
      cap >> frame;
      // If the frame is empty, break immediately
      if (frame.empty())
        break;
      // Display origin image
      ui_main->OriginVideo->showImage(frame);

      cvtColor(frame,m_srcGray,COLOR_BGR2GRAY);
      m_dst.create( frame.size(), frame.type() );
      blur(m_srcGray,m_edge,Size(3,3));
      Canny(m_edge,m_edge,60,60*3,3);
      ui_main->EgdeVideo->showImage(m_edge);
      m_dst = Scalar::all(0);
      frame.copyTo( m_dst, m_edge);
//      ui->processed1View->showImage(m_dst);
      QThread::msleep(30);
      if (stop ==true)
          break;
    }

    // When everything done, release the video capture object
    isProcess =false;
    cap.release();
}

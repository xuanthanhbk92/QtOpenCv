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
    // Check error
    if (ui_main->ldtVideoPath->text().isEmpty()) {
        return;
    }
    if (isProcess == true) {
        return;
    }
    isProcess = true;
    // Get workflow
    VideoCapture cap(ui_main->ldtVideoPath->text().toStdString());
    if(!cap.isOpened()) {
        qDebug() << "Error opening video stream or file";
        return ;
    }
    while(1) {
        Mat frame,grayFrame,equalizeHistFrame, smoothingFrame,edgeDetectFrame,m_dst;
        // Capture frame-by-frame
        cap >> frame;
        // If the frame is empty, break immediately
        if (frame.empty()) {
            qDebug() << "[ERROR] EMPTY FRAME";
            break;
        }
        // Display origin image
        ui_main->OriginVideo->showImage(frame);
        // Convert to gray frame
        cvtColor(frame,grayFrame,COLOR_BGR2GRAY);
        /// Apply Histogram Equalization
        if (ui_main->cbEnhanceMethod->isChecked()) {
            equalizeHist( grayFrame,equalizeHistFrame );
        } else {
            equalizeHistFrame =grayFrame;
        }
        ui_main->VideoEnhance->showImage(equalizeHistFrame);
//         ui_main->OriginVideo->showImage(frame);
        // Apply smoothing filter
        if (ui_main->cbSmoothingMethod->isChecked()) {
            int kernelSize = ui_main->cbSmoothKernelSize->currentText().toInt();
            blur(equalizeHistFrame,smoothingFrame,Size(kernelSize,kernelSize));
        } else {
            smoothingFrame =equalizeHistFrame;
        }
        ui_main->VideoSmooth->showImage(smoothingFrame);
        // Appy edge detection
        if (ui_main->cbCannyEdgeDetection->isChecked()) {
            int kernelSize = ui_main->cbEdgeDetectionKernelSize->currentText().toInt();
            int lowThreshold = ui_main->spnEdgeDetectCannyLowThreshold->value();
            int ratio =3;
            Canny(smoothingFrame,edgeDetectFrame,lowThreshold,lowThreshold*ratio,kernelSize);
        } else {
            edgeDetectFrame =smoothingFrame;
        }
        ui_main->EgdeVideo->showImage(edgeDetectFrame);
//        m_dst.create( frame.size(), frame.type() );
//        m_dst = Scalar::all(0);
//        frame.copyTo( m_dst, edgeDetectFrame);
        // ui->processed1View->showImage(m_dst);
        QThread::msleep(1000/50);
        if (stop ==true) {
            qDebug() << "[INFO] STOP VIDEO";
            break;
        }
    }
    // When everything done, release the video capture object
    isProcess =false;
    cap.release();
}

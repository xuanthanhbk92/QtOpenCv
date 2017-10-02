#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <QDebug>
#include <iostream>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include "videothread.h"
using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectionSetup();
// uncomment this one to enable stylesheet
//   setStyleSheet();
#if (DEBUG == 1)
    /* For debug only */
    ui->lblFileName->setText("/home/thanh/Desktop/Anh.jpg");
    onLoadImage("/home/thanh/Desktop/Anh.jpg");
#endif
#if (DEBUG == 1)
    m_imageLocation = "/home/thanh/Desktop/ProgramPlace/OpenCV/image";
#else
    m_imageLocation = QDir::currentPath();
#endif
    ui->barThesh->setValue(m_lowThreshHold);
    ui->barKernelSize->setValue(m_kernelSize);
    ui->barRatio->setValue(m_ratio);
    // disable gbAdvanceSetting
//    ui->gbAdvanceSetting->setChecked(false);
    ui->gbAdvanceSetting->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnPickFile_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                    tr("Open Image"), m_imageLocation, tr("Image Files (*.png *.jpg *.bmp)"));
    ui->lblFileName->setText(fileName);
    m_imageLocation = QFileInfo(fileName).absolutePath();
    if (!fileName.isEmpty()) {
        onLoadImage(fileName);
    }
}

void MainWindow::onLoadImage(QString fileName)
{
    m_src = cv::imread(fileName.toStdString(), 1 );
    ui->originView->showImage(m_src);
    onShowHistogram(m_src,ui->histogramView);
    onEdgeDetection(m_src);
}

void MainWindow::connectionSetup()
{
    connect(ui->originView,&CQtOpenCVViewerGl::imageSizeChanged,[this](int w,int h) {
        // qDebug() << "Image size:" <<w <<" : " << h;
    });
}

void MainWindow::setStyleSheet(int type)
{
    qDebug("Set style sheet");
//    QFile f(":qdarkstyle/style_deepblue.qss");
    QFile f;
    if (type == SKIN::BLACK)
        f.setFileName(":qdarkstyle/style.qss");
    else if (type == SKIN::DEEPBLUE)
         f.setFileName(":qdarkstyle/style_deepblue.qss");
    else
          f.setFileName("");

    if (!f.exists()) {
        qWarning("Set stylesheet to default");
        qApp->setStyleSheet("");
    } else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}

void MainWindow::onEdgeDetection(cv::Mat &image)
{
    // convert to gray
    if (ui->lblFileName->text().isEmpty())
        return;
    cvtColor(image,m_srcGray,COLOR_BGR2GRAY);
    m_dst.create( image.size(), image.type() );
    blur(m_srcGray,m_edge,Size(3,3));
    Canny(m_edge,m_edge,m_lowThreshHold,m_lowThreshHold*m_ratio,m_kernelSize);
    ui->processedView->showImage(m_edge);
    m_dst = Scalar::all(0);
    m_src.copyTo( m_dst, m_edge);
    ui->processed1View->showImage(m_dst);
}

void MainWindow::on_barThesh_sliderReleased()
{
    m_lowThreshHold =  ui->barThesh->value();
    onEdgeDetection(m_src);
}

void MainWindow::on_actionQuit_triggered()
{
    if (m_videoThread != 0)
        m_videoThread->exitVideo();
    QThread::sleep(1);
    qApp->quit();
}

void MainWindow::on_barRatio_sliderReleased()
{
    m_ratio =ui->barRatio->value();
    onEdgeDetection(m_src);
}

void MainWindow::on_barKernelSize_sliderReleased()
{
    m_kernelSize = ui->barKernelSize->value();
    // force kernel size odd
    if (!(m_kernelSize%2)) {
        m_kernelSize+=1;
        ui->barKernelSize->setValue(m_kernelSize);
    }
    onEdgeDetection(m_src);
}

void MainWindow::onShowHistogram(Mat &image, CQtOpenCVViewerGl *view)
{
    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( image, bgr_planes );
    /// Establish the number of bins
    int histSize = 256;
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    Mat b_hist, g_hist, r_hist;
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    // Draw the histograms for B, G and R
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    static Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ ) {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    view->showImage(histImage);
}

void MainWindow::on_btnVideoBrowse_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                    tr("Open Video"), m_imageLocation, tr("Video Files (*.avi *.mp4)"));
    ui->ldtVideoPath->setText(fileName);
    m_videoLocation = QFileInfo(fileName).absolutePath();
}

void MainWindow::on_btnVideoProcess_clicked()
{
    if (!m_videoLocation.isEmpty()) {
        m_videoThread = new VideoThread(ui,this);
        m_videoThread->start();
    }
}

void MainWindow::on_btnStopVideo_clicked()
{
    if (m_videoThread != 0)
        m_videoThread->exitVideo();
}

void MainWindow::on_actionDeepBlue_triggered()
{
    setStyleSheet(SKIN::DEEPBLUE);
}

void MainWindow::on_actionBlack_triggered()
{
    setStyleSheet(SKIN::BLACK);
}

void MainWindow::on_actionDefault_triggered()
{
    setStyleSheet(SKIN::DEFAULT);
}


void MainWindow::on_btnAdvanceSetting_toggled(bool checked)
{
  if(checked){
      ui->gbAdvanceSetting->show();
  }  else {
      ui->gbAdvanceSetting->hide();
  }
}

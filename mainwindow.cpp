#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>
#include <iostream>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    cv::Mat image = cv::imread("/home/thanh/Desktop/Anh.jpg", 1 );
//    cout << "R (c)       = " << endl << format(image,"C"     ) << endl << endl;
//    create image window named "My Image"
//    cv::namedWindow("My Image");
//    show the image on window
//    cv::imshow("My Image", image);
    /* Show image via custom qt widget */
//    ui->myView->showImage(image);
    connectionSetup();
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
    onEdgeDetection(m_src);
}

void MainWindow::connectionSetup()
{
    connect(ui->originView,&CQtOpenCVViewerGl::imageSizeChanged,[this](int w,int h) {
       // qDebug() << "Image size:" <<w <<" : " << h;
    });
}

void MainWindow::onEdgeDetection(cv::Mat &image)
{
    // convert to gray
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
    onEdgeDetection(m_src);
}

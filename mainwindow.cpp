#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>
#include <iostream>
#include <QFileDialog>
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


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnPickFile_clicked()
{
    QString appDir = QDir::currentPath();
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), appDir, tr("Image Files (*.png *.jpg *.bmp)"));

    ui->lblFileName->setText(fileName);
    if (!fileName.isEmpty()){
        onLoadImage(fileName);
    }
}

void MainWindow::onLoadImage(QString fileName)
{
    src = cv::imread(fileName.toStdString(), 1 );
    ui->originView->showImage(src);
    onEdgeDetection(src);
}

void MainWindow::connectionSetup()
{
    connect(ui->originView,&CQtOpenCVViewerGl::imageSizeChanged,[this](int w,int h){
        qDebug() << "Image size:" <<w <<" : " << h;
    });
}

void MainWindow::onEdgeDetection(cv::Mat &image)
{
    // convert to gray
    cvtColor(image,srcGray,COLOR_BGR2GRAY);
    int ratio = 3;
    int kernel_size = 3;
    dst.create( image.size(), image.type() );
    blur(srcGray,edge,Size(3,3));
    Canny(edge,edge,lowThreshold,lowThreshold*ratio,kernel_size);
    ui->processedView->showImage(edge);
    dst = Scalar::all(0);
    src.copyTo( dst, edge);
    ui->processed1View->showImage(dst);
}

void MainWindow::on_barThesh_sliderReleased()
{
    lowThreshold =  ui->barThesh->value();
    onEdgeDetection(src);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

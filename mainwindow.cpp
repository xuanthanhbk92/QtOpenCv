#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cv::Mat image = cv::imread("/home/thanh/Desktop/Anh.jpg", 1 );
     // create image window named "My Image"
//    cv::namedWindow("My Image");
//       // show the image on window
//    cv::imshow("My Image", image);

    ui->myView->showImage(image);

    connect(ui->myView,&CQtOpenCVViewerGl::imageSizeChanged,[this](int w,int h){
        qDebug() << "Image size:" <<w <<" : " << h;
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

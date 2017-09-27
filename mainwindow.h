#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include"cqtopencvviewergl.h"
#include <opencv2/opencv.hpp>
#include "videothread.h"
#define DEBUG 1

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum SKIN {
        BLACK =0 ,
        DEEPBLUE =1,
        DEFAULT =2
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
private slots:
    void onLoadImage(QString fileName);
    void onEdgeDetection(cv::Mat &image);
    void on_btnPickFile_clicked();
    void on_barThesh_sliderReleased();
    void on_actionQuit_triggered();
    void on_barRatio_sliderReleased();
    void on_barKernelSize_sliderReleased();
    void onShowHistogram(cv::Mat& image,CQtOpenCVViewerGl* view);
    void on_btnVideoBrowse_clicked();
    void on_btnVideoProcess_clicked();
    void on_btnStopVideo_clicked();
    void on_actionDeepBlue_triggered();

    void on_actionBlack_triggered();

    void on_actionDefault_triggered();

private:

    void connectionSetup();
    void setStyleSheet(int type);

    /* Canny variable */
    QString m_imageLocation;
    cv::Mat m_src,m_dst,m_edge,m_srcGray;
    QString m_videoLocation;
    int m_lowThreshHold =20;
    int m_ratio = 3;
    int m_kernelSize =3;

    VideoThread * m_videoThread = nullptr;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>

#define DEBUG 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onLoadImage(QString fileName);
    void onEdgeDetection(cv::Mat &image);
    void on_btnPickFile_clicked();
    void on_barThesh_sliderReleased();
    void on_actionQuit_triggered();
    void on_barRatio_sliderReleased();
    void on_barKernelSize_sliderReleased();

private:

    void connectionSetup();
    Ui::MainWindow *ui;
    /* Canny variable */
    QString m_imageLocation;
    cv::Mat m_src,m_dst,m_edge,m_srcGray;
    int m_lowThreshHold =20;
    int m_ratio = 3;
    int m_kernelSize =3;

};

#endif // MAINWINDOW_H

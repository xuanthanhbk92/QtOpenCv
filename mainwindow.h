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
    void on_btnPickFile_clicked();
    void onLoadImage(QString fileName);
    void onEdgeDetection(cv::Mat &image);
    void on_barThesh_sliderReleased();

    void on_actionQuit_triggered();

private:

    void connectionSetup();

    Ui::MainWindow *ui;
    cv::Mat src,dst,edge,srcGray;
    int lowThreshold =10;

};

#endif // MAINWINDOW_H

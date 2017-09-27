#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QObject>
#include <QThread>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}


class VideoThread : public QThread
{
    Q_OBJECT
public:
    explicit VideoThread(Ui::MainWindow* main,QObject *parent = 0);
    void exitVideo();
signals:

public slots:

    // QThread interface
protected:
    void run() override;
private:
   Ui::MainWindow * ui_main;
   static bool isProcess;
   bool stop =false;
};

#endif // VIDEOTHREAD_H

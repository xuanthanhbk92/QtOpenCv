#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QObject>

class VideoThread : public QObject
{
    Q_OBJECT
public:
    explicit VideoThread(QObject *parent = 0);

signals:

public slots:
};

#endif // VIDEOTHREAD_H
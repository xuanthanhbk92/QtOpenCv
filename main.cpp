#include "mainwindow.h"
#include <QApplication>


#define APP_TITLE "Digital Image Processing"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(APP_TITLE);
    w.show();

    return a.exec();
}

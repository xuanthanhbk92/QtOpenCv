#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T10:56:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCV
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cqtopencvviewergl.cpp

HEADERS  += mainwindow.h \
    cqtopencvviewergl.h

FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += opencv

#LIBS += -L/usr/lib/x86_64-linux-gnu
#    -lopencv_core \
#    -lopencv_highgui \
#    -lopencv_imgproc \
#    -lopencv_features2d \
#    -lopencv_calib3d \
#LIBS += -lopencv_core
#LIBS += -lopencv_imgproc
#LIBS += -lopencv_highgui
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
#LIBS += -lopencv_features2d
#LIBS += -lopencv_calib3d
#LIBS += -lopencv_objdetect
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#LIBS += -lopencv_flann

RESOURCES += \
    res/res.qrc

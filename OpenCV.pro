#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T10:56:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCV
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    cqtopencvviewergl.cpp

HEADERS  += mainwindow.h \
    cqtopencvviewergl.h

FORMS    += mainwindow.ui

win32 {
message("Using win32 configuration")
OPENCV_PATH = C:/opencv/    # Note: update with the correct OpenCV version
#LIBS_PATH = "$$OPENCV_PATH/build/x86/mingw/lib" #project compiled using MINGW
LIBS_PATH = "$$OPENCV_PATH/build/x86/vc10/lib" #project compiled using Visual C++ 2010 32bit compiler

    CONFIG(debug, debug|release) {
    LIBS     += -L$$LIBS_PATH \
                -lopencv_core246d \
                -lopencv_highgui246d
    }

    CONFIG(release, debug|release) {
    LIBS     += -L$$LIBS_PATH \
                -lopencv_core246 \
                -lopencv_highgui246
    }
INCLUDEPATH += \
    $$OPENCV_PATH/modules/core/include/ \ #core module
    $$OPENCV_PATH/modules/highgui/include/ #highgui modul

message("OpenCV path: $$OPENCV_PATH")
message("Includes path: $$INCLUDEPATH")
message("Libraries: $$LIBS")
}

unix {
message("Using unix configuration")
CONFIG += link_pkgconfig
PKGCONFIG += opencv
}






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

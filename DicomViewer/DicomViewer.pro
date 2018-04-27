#-------------------------------------------------
#
# Project created by QtCreator 2018-04-25T10:43:07
#
#-------------------------------------------------

QT       += core gui

DEFINES  -= UNICODE




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DicomViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        dicomviewer.cpp

HEADERS += \
        dicomviewer.h

FORMS += \
        dicomviewer.ui


INCLUDEPATH += \
            "C:/Program Files/DCMTK/include"

CONFIG（debug，debug | release）{
LIBS += -L"C:/Program Files/DCMTK/lib"\
    -ladvapi32 \
    -lzlib_d \
    -ldcmdata \
    -lijg16 \
    -lijg8 \
    -lijg12 \
    -ldcmimgle \
    -ldcmimage \
    -ldcmjpeg \
    -loflog \
    -lofstd \
    -lws2_32 \
    -lnetapi32 \
    -lwsock32
} else {
    LIBS+=  -L"C:/Program Files/DCMTK/lib"\
    -ladvapi32 \
    -lzlib_o \
    -ldcmdata \
    -lijg16 \
    -lijg8 \
    -lijg12 \
    -ldcmimgle \
    -ldcmimage \
    -ldcmjpeg \
    -loflog \
    -lofstd \
    -lws2_32 \
    -lnetapi32 \
    -lwsock32 \
}
INCLUDEPATH += "C:/opencv/opencv/build/include" \
               "C:/opencv/opencv/build/include/opencv2" \
               "C:/opencv/opencv/build/include/opencv"
CONFIG(release,debug|release){
    LIBS += "C:\opencv\opencv\build\x64\vc14\lib\opencv_world341.lib"
}
CONFIG(debug,debug|release){
    LIBS += "C:\opencv\opencv\build\x64\vc14\lib\opencv_world341d.lib"
}


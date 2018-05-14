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
    DcmEnhance.cpp

HEADERS += \
         DcmEnhance.h \
         libMyAdd.h

FORMS += \
         DcmEnhance.ui


INCLUDEPATH += "D:/DCMTK/include"

CONFIG(debug,debug|release){
LIBS += -l"D:/DCMTK/lib/zlib_d" \
    -l"D:/DCMTK/lib/dcmdata_d" \
    -l"D:/DCMTK/lib/ijg16_d" \
    -l"D:/DCMTK/lib/ijg8_d" \
    -l"D:/DCMTK/lib/ijg12_d" \
    -l"D:/DCMTK/lib/dcmimgle_d" \
    -l"D:/DCMTK/lib/dcmimage_d" \
    -l"D:/DCMTK/lib/dcmjpeg_d" \
    -l"D:/DCMTK/lib/oflog_d" \
    -l"D:/DCMTK/lib/ofstd_d" \
    -l"ws2_32"\
    -l"netapi32" \
    -l"wsock32"\
    -l"advapi32"\
}else{
LIBS += -l"D:/DCMTK/lib/zlib_o" \
    -l"D:/DCMTK/lib/dcmdata" \
    -l"D:/DCMTK/lib/ijg16" \
    -l"D:/DCMTK/lib/ijg8" \
    -l"D:/DCMTK/lib/ijg12" \
    -l"D:/DCMTK/lib/dcmimgle" \
    -l"D:/DCMTK/lib/dcmimage" \
    -l"D:/DCMTK/lib/dcmjpeg" \
    -l"D:/DCMTK/lib/oflog" \
    -l"D:/DCMTK/lib/ofstd" \
    -l"ws2_32"\
    -l"netapi32" \
    -l"wsock32"\
    -l"advapi32"\
}



INCLUDEPATH += "D:\Matlab\extern\include"
INCLUDEPATH += "D:\MatLab\extern\include\win64"

OTHER_FILES +=libMyAdd.dll\
              libMyAdd.lib


LIBS += "F:/QTProject/DcmEnhance/libMyAdd.lib"

INCLUDEPATH += "D:/opencv/build/include" \
               "D:/opencv/build/include/opencv2" \
               "D:/opencv/build/include/opencv"

CONFIG(release,debug|release){
    LIBS += "D:/opencv/build/x64/vc14/lib/opencv_world341.lib"
}
CONFIG(debug,debug|release){
    LIBS += "D:/opencv/build/x64/vc14/lib/opencv_world341d.lib"
}

LIBS += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/iphlpapi.lib"

LIBS += "D:/Matlab/extern/lib/win64/microsoft/mclmcrrt.lib"


#include "dicomviewer.h"
#include <QApplication>

#pragma comment(lib, "Iphlpapi.lib")


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DicomViewer w;
    w.show();

    return a.exec();
}

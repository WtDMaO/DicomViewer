#include "dcmenhance.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DcmEnhance w;
    w.show();

    return a.exec();
}

#include "QTOSG.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTOSG w;
    w.show();
    return a.exec();
}

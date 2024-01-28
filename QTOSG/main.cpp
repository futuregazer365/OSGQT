#include "QTOSG.h"
#include <QtWidgets/QApplication>
#include "QtOSGWidget.h"
int main(int argc, char *argv[])
{
	#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
		QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
	#else
		qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray("1"));
	#endif

    QApplication a(argc, argv);
    QTOSG w;
	QtOSGWidget *widget = new QtOSGWidget(&w);
	w.setCentralWidget(widget);
	w.Centralwidget = widget;
	w.show();
    return a.exec();
}

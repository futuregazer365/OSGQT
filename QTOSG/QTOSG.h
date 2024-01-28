#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTOSG.h"
#include "QtOSGWidget.h"
class QTOSG : public QMainWindow
{
    Q_OBJECT

public:
    QTOSG(QWidget *parent = Q_NULLPTR);
	QtOSGWidget *Centralwidget = nullptr;

public slots:
	void onAddModeltoViewer();
protected:
	virtual void keyPressEvent(QKeyEvent* event);
private:
    Ui::QTOSGClass ui;
};

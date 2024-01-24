#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTOSG.h"

class QTOSG : public QMainWindow
{
    Q_OBJECT

public:
    QTOSG(QWidget *parent = Q_NULLPTR);

private:
    Ui::QTOSGClass ui;
};

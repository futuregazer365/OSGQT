#include "QTOSG.h"
#include <QFileDialog>
#include "QtOSGWidget.h"
#include <osgDB/ReadFile>
QTOSG::QTOSG(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void QTOSG::onAddModeltoViewer() 
{
	QFileDialog dlg(this, QStringLiteral("添加模型"));//构造文件对话框
	dlg.setAcceptMode(QFileDialog::AcceptOpen);//文件对话框为打开类型
	dlg.setFileMode(QFileDialog::ExistingFile);//单文件

	QStringList filters;//文本过滤
	filters << QStringLiteral("模型(*.osg)");//只显示osg后缀的文件
	dlg.setNameFilters(filters);
	dlg.exec();//显示窗口，阻塞当前线程，等待用户操作

	QStringList files = dlg.selectedFiles();//获取用户选择文件路径

	if (files.isEmpty()) return;//文件路径为空则返回
	if (Centralwidget) //中央窗口指针判空（QtOSGWidget）
	{
		std::string str = files.at(0).toStdString();//QString转string
		Centralwidget->geode->removeDrawables(0, Centralwidget->geode->getNumDrawables());//移除圆柱体节点
		osg::Node *node = osgDB::readNodeFile(str);//读取str路径的文件，返回为节点
		Centralwidget->root->addChild(node);//节点加入到视景树根
		Centralwidget->_mViewer->home();//更改摄像机到全景
	}
}

void QTOSG::keyPressEvent(QKeyEvent * event)
{
	//event->ignore();
}

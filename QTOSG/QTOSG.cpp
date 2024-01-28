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
	QFileDialog dlg(this, QStringLiteral("���ģ��"));//�����ļ��Ի���
	dlg.setAcceptMode(QFileDialog::AcceptOpen);//�ļ��Ի���Ϊ������
	dlg.setFileMode(QFileDialog::ExistingFile);//���ļ�

	QStringList filters;//�ı�����
	filters << QStringLiteral("ģ��(*.osg)");//ֻ��ʾosg��׺���ļ�
	dlg.setNameFilters(filters);
	dlg.exec();//��ʾ���ڣ�������ǰ�̣߳��ȴ��û�����

	QStringList files = dlg.selectedFiles();//��ȡ�û�ѡ���ļ�·��

	if (files.isEmpty()) return;//�ļ�·��Ϊ���򷵻�
	if (Centralwidget) //���봰��ָ���пգ�QtOSGWidget��
	{
		std::string str = files.at(0).toStdString();//QStringתstring
		Centralwidget->geode->removeDrawables(0, Centralwidget->geode->getNumDrawables());//�Ƴ�Բ����ڵ�
		osg::Node *node = osgDB::readNodeFile(str);//��ȡstr·�����ļ�������Ϊ�ڵ�
		Centralwidget->root->addChild(node);//�ڵ���뵽�Ӿ�����
		Centralwidget->_mViewer->home();//�����������ȫ��
	}
}

void QTOSG::keyPressEvent(QKeyEvent * event)
{
	//event->ignore();
}

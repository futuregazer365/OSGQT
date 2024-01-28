#pragma once

/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description: Minimalistic project example that uses both Qt and OpenSceneGraph libraries.
 *
 *        Version:  1.0
 *        Created:  30-Jun-16 10:23:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Victoria Rudakova (vicrucann@gmail.com),
 *   Organization:  vicrucann.github.io
 *
 * =====================================================================================
 */
//#include <stdlib.h>
//#include <stdio.h>
#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include <QScreen>
#include <QtGlobal>
#include <QWindow>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>
class QtOSGWidget : public QOpenGLWidget {
public:
	osg::Geode *geode = nullptr;
	osg::Group *root = nullptr;
	osg::ref_ptr<osgViewer::Viewer> _mViewer;
	QtOSGWidget(QWidget *parent = 0): QOpenGLWidget(parent)
		, _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded(this->x(), this->y(),
			this->width(), this->height()))
		, _mViewer(new osgViewer::Viewer)
		// take care of HDPI screen, e.g. Retina display on Mac
		, m_scale(QApplication::desktop()->devicePixelRatio()) 
	{
		osg::Cylinder *cylinder = new osg::Cylinder(osg::Vec3(0.f, 0.f, 0.f), 0.25f, 0.5f);//����Բ����
		osg::ShapeDrawable *sd = new osg::ShapeDrawable(cylinder);//Բ�������������ʾ����
		sd->setColor(osg::Vec4(0.8f, 0.5f, 0.2f, 1.f));//������ɫ
		geode = new osg::Geode;//�����ӽڵ�
		root = new osg::Group;//������ڵ�
		geode->addDrawable(sd);//�ӽڵ����Բ����
		root->addChild(geode);//��ڵ����node�ڵ�

		osg::Camera *camera = new osg::Camera;//�������
		camera->setViewport(0, 0, this->width(), this->height());//�����ӿ�
		camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.f, 1.f));//���ñ���ɫ
		float aspectRatio = static_cast<float>(this->width()) / static_cast<float>(this->height());//���㴰�ڿ�߱�
		camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);//����ͶӰ����
		camera->setGraphicsContext(_mGraphicsWindow);//����ͼ��������

		_mViewer->setCamera(camera);//ʱ�����������
		_mViewer->setSceneData(root);//�Ӿ��������������ڵ�
		osgGA::TrackballManipulator *manipulator = new osgGA::TrackballManipulator;//����������
		manipulator->setAllowThrow(false);//���ý�ֹ������ק
		this->setMouseTracking(true);//QOpenGLWidget����������
		this->setFocusPolicy(Qt::StrongFocus);
		_mViewer->setCameraManipulator(manipulator);//�Ӿ�������������
		_mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);//�Ӿ������߳�ģʽ
		_mViewer->realize();//��ʼ��viewer
	}


	virtual ~QtOSGWidget() {}

protected:
	/*
	��ȾOpenGL��������Ҫ����Widgetʱ�ͻ����
	*/
	virtual void paintGL() {
		_mViewer->frame();
	}
	/*
	����OpenGL�ӿڣ�ͶӰ�ȡ�ÿ������Widget�Ĵ�Сʱ����һ����ʾ����Widgetʱ�����������Ϊ�����´���Widget�����Զ���õ�����С���¼���
	*/
	virtual void resizeGL(int width, int height) {
		this->getEventQueue()->windowResize(this->x()*m_scale, this->y() * m_scale, width * m_scale, height * m_scale);
		_mGraphicsWindow->resized(this->x()*m_scale, this->y() * m_scale, width * m_scale, height * m_scale);
		osg::Camera *camera = _mViewer->getCamera();
		camera->setViewport(0, 0, this->width()*m_scale, this->height()* m_scale);
	}
	/*
	����OpenGL����Դ��״̬���ڵ�һ�ε���resizeGL()��paintGL()֮ǰ����һ��
	*/
	virtual void initializeGL() {
		osg::Group *geode = dynamic_cast<osg::Group *>(_mViewer->getSceneData());
		osg::StateSet *stateSet = geode->getOrCreateStateSet();
		osg::Material *material = new osg::Material;
		material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
		stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
		stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	}

	/*
	��OSG�¼����з�������ƶ��¼�
	*/
	virtual void mouseMoveEvent(QMouseEvent *event) {
		this->getEventQueue()->mouseMotion(event->x()*m_scale, event->y()*m_scale);
	}
	/*
	��OSG�¼����з�����갴���¼�
	*/
	virtual void mousePressEvent(QMouseEvent *event) {
		unsigned int button = 0;
		switch (event->button()) {
		case Qt::LeftButton:
			button = 1;
			break;
		case Qt::MiddleButton:
			button = 2;
			break;
		case Qt::RightButton:
			button = 3;
			break;
		default:
			break;
		}
		this->getEventQueue()->mouseButtonPress(event->x()*m_scale, event->y()*m_scale, button);
	}
	/*
	��OSG�¼����з�����굯���¼�
	*/
	virtual void mouseReleaseEvent(QMouseEvent *event) {
		unsigned int button = 0;
		switch (event->button()) {
		case Qt::LeftButton:
			button = 1;
			break;
		case Qt::MiddleButton:
			button = 2;
			break;
		case Qt::RightButton:
			button = 3;
			break;
		default:
			break;
		}
		this->getEventQueue()->mouseButtonRelease(event->x()*m_scale, event->y()*m_scale, button);
	}
	/*
	��OSG�¼����з����������¼�
	*/
	virtual void wheelEvent(QWheelEvent *event) {
		int delta = event->delta();
		osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
			osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
		this->getEventQueue()->mouseScroll(motion);
	}
	/*
	��OSG�¼����з������˫���¼�
	*/
	virtual void mouseDoubleClickEvent(QMouseEvent * event) 
	{
		unsigned int button = 0;
		switch (event->button()) {
		case Qt::LeftButton:
			button = 1;
			break;
		case Qt::MiddleButton:
			button = 2;
			break;
		case Qt::RightButton:
			button = 3;
			break;
		default:
			break;
		}
		this->getEventQueue()->mouseDoubleButtonPress(event->x()*m_scale, event->y()*m_scale, button);
	}
	/*
	��OSG�¼����з��ͼ��̰����¼�
	*/
	virtual void keyPressEvent(QKeyEvent* event)
	{
		unsigned int button = 0;
		switch (event->key()) {
		case Qt::Key_Up:
			button = osgGA::GUIEventAdapter::KEY_Up;
		}
		this->getEventQueue()->keyPress(button);

	}
	virtual bool event(QEvent *event) {
		bool handled = QOpenGLWidget::event(event);
		this->update();
		return handled;
	}

private:

	osgGA::EventQueue *getEventQueue() const {
		osgGA::EventQueue *eventQueue = _mGraphicsWindow->getEventQueue();
		return eventQueue;
	}

	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;

	qreal m_scale;
};

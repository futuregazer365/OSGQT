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
		osg::Cylinder *cylinder = new osg::Cylinder(osg::Vec3(0.f, 0.f, 0.f), 0.25f, 0.5f);//创建圆柱体
		osg::ShapeDrawable *sd = new osg::ShapeDrawable(cylinder);//圆柱体关联到可显示对象
		sd->setColor(osg::Vec4(0.8f, 0.5f, 0.2f, 1.f));//设置颜色
		geode = new osg::Geode;//创建子节点
		root = new osg::Group;//创建组节点
		geode->addDrawable(sd);//子节点管理圆柱体
		root->addChild(geode);//组节点管理node节点

		osg::Camera *camera = new osg::Camera;//创建相机
		camera->setViewport(0, 0, this->width(), this->height());//设置视口
		camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.f, 1.f));//设置背景色
		float aspectRatio = static_cast<float>(this->width()) / static_cast<float>(this->height());//计算窗口宽高比
		camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);//设置投影矩阵
		camera->setGraphicsContext(_mGraphicsWindow);//设置图形上下文

		_mViewer->setCamera(camera);//时景器关联相机
		_mViewer->setSceneData(root);//视景器关联场景树节点
		osgGA::TrackballManipulator *manipulator = new osgGA::TrackballManipulator;//创建操作器
		manipulator->setAllowThrow(false);//设置禁止惯性拖拽
		this->setMouseTracking(true);//QOpenGLWidget启动鼠标跟踪
		this->setFocusPolicy(Qt::StrongFocus);
		_mViewer->setCameraManipulator(manipulator);//视景器关联操作器
		_mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);//视景器单线程模式
		_mViewer->realize();//初始化viewer
	}


	virtual ~QtOSGWidget() {}

protected:
	/*
	渲染OpenGL场景，需要更新Widget时就会调用
	*/
	virtual void paintGL() {
		_mViewer->frame();
	}
	/*
	设置OpenGL视口，投影等。每当调整Widget的大小时（第一次显示窗口Widget时会调用它，因为所有新创建Widget都会自动获得调整大小的事件）
	*/
	virtual void resizeGL(int width, int height) {
		this->getEventQueue()->windowResize(this->x()*m_scale, this->y() * m_scale, width * m_scale, height * m_scale);
		_mGraphicsWindow->resized(this->x()*m_scale, this->y() * m_scale, width * m_scale, height * m_scale);
		osg::Camera *camera = _mViewer->getCamera();
		camera->setViewport(0, 0, this->width()*m_scale, this->height()* m_scale);
	}
	/*
	建立OpenGL的资源和状态。在第一次调用resizeGL()或paintGL()之前调用一次
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
	向OSG事件队列发送鼠标移动事件
	*/
	virtual void mouseMoveEvent(QMouseEvent *event) {
		this->getEventQueue()->mouseMotion(event->x()*m_scale, event->y()*m_scale);
	}
	/*
	向OSG事件队列发送鼠标按下事件
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
	向OSG事件队列发送鼠标弹起事件
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
	向OSG事件队列发送鼠标滚轮事件
	*/
	virtual void wheelEvent(QWheelEvent *event) {
		int delta = event->delta();
		osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
			osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
		this->getEventQueue()->mouseScroll(motion);
	}
	/*
	向OSG事件队列发送鼠标双击事件
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
	向OSG事件队列发送键盘按下事件
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

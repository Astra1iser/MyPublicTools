#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyPublicQtTools.h"
#include "stdafx.h"

//#include "..\..\Include\QTInclude\MyQHeaderView\MyQHeaderView.h"
//#include "..\..\Include\QTInclude\MyQTTitleBar\MyTitleBar.h"

#include "MyTitleBar.h"


#include <QWidget>
#include <dwmapi.h>
#include "qgraphicseffect.h"
#include <windowsx.h>
#include <qt_windows.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "user32.lib")


class MyPublicQtTools : public QMainWindow
{
    Q_OBJECT

public:
    MyPublicQtTools(QWidget *parent = Q_NULLPTR);
    ~MyPublicQtTools()
    {
        //delete ui;
    }


protected:
	MyTitleBar* m_titleBar;
    //void paintEvent(QPaintEvent* event);
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);

private:
	void initTitleBar();									//初始化标题



private:
    Ui::MyPublicQtToolsClass ui;
};






//#pragma once
//
//#include <QWidget>
//#include "MuCustomWindow.h"
//#include "ui_MyPublicQtTools.h"
//#include "stdafx.h"
//
////#include "..\..\Include\QTInclude\MyQHeaderView\MyQHeaderView.h"
////#include "..\..\Include\QTInclude\MyQTTitleBar\MyTitleBar.h"
//
////#include "MyTitleBar.h"
//
//
//
//
//
//class MyPublicQtTools : public MuCustomWindow
//{
//    Q_OBJECT
//
//public:
//    MyPublicQtTools(QWidget* parent = Q_NULLPTR);
//    ~MyPublicQtTools()
//    {
//        //delete ui;
//    }
//
////protected:
////    bool nativeEvent(const QByteArray& eventType, void* message, long* result);
//
//
////protected:
////    MyTitleBar* m_titleBar;
////
////
////private:
////    void initTitleBar();									//初始化标题
//
//
//
//private:
//    Ui::MyPublicQtToolsClass ui;
//};

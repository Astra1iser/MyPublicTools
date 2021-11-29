#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyPublicQtTools.h"
#include "stdafx.h"

//#include "..\..\Include\QTInclude\MyQHeaderView\MyQHeaderView.h"
//#include "..\..\Include\QTInclude\MyQTTitleBar\MyTitleBar.h"

#include "MyTitleBar.h"


#include <QWidget>


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
    void paintEvent(QPaintEvent* event);
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);


private:
	void initTitleBar();									//初始化标题


private:
    Ui::MyPublicQtToolsClass ui;
};

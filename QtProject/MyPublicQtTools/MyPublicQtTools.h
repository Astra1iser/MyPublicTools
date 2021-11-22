#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyPublicQtTools.h"
#include "stdafx.h"

//#include "..\..\Include\QTInclude\MyQHeaderView\MyQHeaderView.h"
//#include "..\..\Include\QTInclude\MyQTTitleBar\MyTitleBar.h"

#include "MyTitleBar.h"



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

private:
	void initTitleBar();									//��ʼ������




private:
    Ui::MyPublicQtToolsClass ui;
};

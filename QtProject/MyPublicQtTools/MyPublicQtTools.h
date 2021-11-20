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
	void initTitleBar();									//初始化标题

//private slots:
//	void onButtonMinClicked();								//最小化槽函数
//	void onButtonRestoreClicked();							//还原键槽函数
//	void onButtonMaxClicked();								//最大化槽函数
//	void onButtonCloseClicked();							//关闭按钮槽函数 */


private:
    Ui::MyPublicQtToolsClass ui;
};

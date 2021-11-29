#pragma   once
#ifndef MYFRAME_H
#define MYFRAME_H
#include <QtWidgets>
#include <QWidget>
#include <QtWidgets/QDialog>
#include <QLabel>

#include "stdafx.h"

class MyFrame : public QWidget
{
	Q_OBJECT

public:
	MyFrame(QWidget* parent);
	// 这里parent没有给默认值NULL，保证在创建MyFrame对象时父指针必须得赋值;且赋值不为NULL;
	~MyFrame();

	QFrame* m_Top_Horizontal_Line;
	QFrame* m_Bottom_Horizontal_Line;
	QFrame* m_Left_Vertical_Line;
	QFrame* m_Right_Vertical_Line;
};
#endif
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
	// ����parentû�и�Ĭ��ֵNULL����֤�ڴ���MyFrame����ʱ��ָ�����ø�ֵ;�Ҹ�ֵ��ΪNULL;
	~MyFrame();

	QFrame* m_Top_Horizontal_Line;
	QFrame* m_Bottom_Horizontal_Line;
	QFrame* m_Left_Vertical_Line;
	QFrame* m_Right_Vertical_Line;
};
#endif
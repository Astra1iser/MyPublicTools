#include "MyFrame.h"

MyFrame::MyFrame(QWidget* parent)
{
	//上边界
	m_Top_Horizontal_Line = new QFrame(parent);
	m_Top_Horizontal_Line->setObjectName(QStringLiteral("Top_Horizontal_Line"));
	m_Top_Horizontal_Line->setFrameShape(QFrame::HLine);
	m_Top_Horizontal_Line->setFrameShadow(QFrame::Sunken);
	//m_Top_Horizontal_Line->setGeometry(QRect(0, 0, parent->width(), 3));


	//下边界
	m_Bottom_Horizontal_Line = new QFrame(parent);
	m_Bottom_Horizontal_Line->setObjectName(QStringLiteral("Bottom_Horizontal_Line"));
	m_Bottom_Horizontal_Line->setFrameShape(QFrame::HLine);
	m_Bottom_Horizontal_Line->setFrameShadow(QFrame::Sunken);
	//m_Bottom_Horizontal_Line->setGeometry(QRect(0, parent->height(), parent->width(), 3));


	//左边界
	m_Left_Vertical_Line = new QFrame(parent);
	m_Left_Vertical_Line->setObjectName(QStringLiteral("Left_Vertical_Line"));
	m_Left_Vertical_Line->setFrameShape(QFrame::VLine);
	m_Left_Vertical_Line->setFrameShadow(QFrame::Sunken);
	//m_Left_Vertical_Line->setGeometry(QRect(0, 0, 1, parent->height()));


	//右边界
	m_Right_Vertical_Line = new QFrame(parent);
	m_Right_Vertical_Line->setObjectName(QStringLiteral("Right_Vertical_Line"));
	m_Right_Vertical_Line->setFrameShape(QFrame::VLine);
	m_Right_Vertical_Line->setFrameShadow(QFrame::Sunken);
	//m_Right_Vertical_Line->setGeometry(QRect(parent->width(), 0, 3, parent->height()));



}

MyFrame::~MyFrame()
{
	delete m_Top_Horizontal_Line;
	delete m_Bottom_Horizontal_Line;
	delete m_Left_Vertical_Line;
	delete m_Right_Vertical_Line;
}
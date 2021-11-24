#pragma   once
#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H
#include <QtWidgets>
#include <QWidget>
#include <QtWidgets/QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "stdafx.h"
enum ButtonType
{
	MIN_BUTTON = 0,         // 最小化和关闭按钮;
	MIN_MAX_BUTTON ,        // 最小化、最大化和关闭按钮;
	ONLY_CLOSE_BUTTON       // 只有关闭按钮;
};

enum MouseClickTimes
{
	NOCLICK = 0,
	SINGLECLICK,
	DOUBLECLICK,
	OTHERTIMESCLICK
};

//待开发功能 贴边毛玻璃效果(这个貌似需要封装一个窗体类,不要在标题类上做了)
//焦点问题
//图标左右键菜单


//ps:我觉得我接下来要参考这个了https://blog.csdn.net/a844651990/article/details/106985554/


//如何使用这个自定义标题栏?

//首先需要在窗体中初始化自定义标题栏对象
/* 
protected:
	MyTitleBar* m_titleBar;
	
private:
	void initTitleBar();									//初始化自定义标题栏
	
*/




//然后在主窗体中初始化这个方法(一定要在 ui.setupUi(this); 后)
/* void QAXSafeCheckerTrustDlg ::initTitleBar()
{
	m_titleBar = new MyTitleBar(this);
	m_titleBar->setTitleIcon("../Image/QAXico.png");
	m_titleBar->setTitleContent(QStringLiteral(" 这是标题"),10);
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());



} */

class MyTitleBar : public QDialog
{
	Q_OBJECT

public:
	MyTitleBar(QWidget *parent);
	//这里parent没有给默认值NULL，保证在创建MyTitleBar对象时父指针必须得赋值;且赋值不为NULL;
	~MyTitleBar();

	// 设置标题栏背景色及是否设置标题栏背景色透明;
	void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
	// 设置标题栏图标;
	void setTitleIcon(QString filePath , QSize IconSize = QSize(25 , 25));
	// 设置标题内容;
	void setTitleContent(QString titleContent , int titleFontSize = 9, const char* ColorStyle = "color:white;");
	// 设置标题栏长度;
	void setTitleWidth(int width = 0);
	// 设置标题栏高度;
	void setTitleHeight(int height = 0);
	// 设置标题栏上按钮类型;
	void setButtonType(ButtonType buttonType);
	// 设置标题栏中的标题是否会滚动(值不要太小,会增加Cpu消耗);
	void setTitleRoll(int timeInterval = 5);
	// 设置窗口边框宽度(这里是填父窗体中边框的长度,设置后,标题栏的左右和上方会空出对应大小的宽度用于显示父窗口边框);
	void setWindowBorderWidth(int borderWidth);
	//设置右键菜单
	void setRightClickMenu();

private:
	void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	// 初始化控件;
	void initControl();
	// 信号槽的绑定;
	void initConnections();
	// 加载样式文件;
	void loadStyleSheet(const QString &sheetName);

signals:


private slots:
	//判断单双击的计时器槽函数
	void slotTimerTimeOut();

	void _SlotPlayArgsMenu(const QPoint pos);

	// 按钮触发的槽;
	void onButtonMinClicked();
	void onButtonMinPressed();
	void onButtonMaxClicked();
	void onButtonMaxPressed();
	void onButtonRestoreClicked();
	void onButtonRestorePressed();
	void onButtonCloseClicked();
	void onButtonClosePressed();
	void onRollTitle();
	void onIconClicked();
	void onIconPressed();





private:
	QLabel* m_pIcon;                    // 标题栏图标;
	QLabel* m_pTitleContent;            // 标题栏内容;
	QLabel* m_pTitleContent2;            // 标题栏内容;
	QPushButton* m_pButtonMin;          // 最小化按钮;
	QPushButton* m_pButtonRestore;      // 最大化还原按钮;
	QPushButton* m_pButtonMax;          // 最大化按钮;
	QPushButton* m_pButtonClose;        // 关闭按钮;

	// 标题栏背景色;
	int m_colorR;
	int m_colorG;
	int m_colorB;

	// 最大化，最小化变量;
	QPoint m_restorePos;
	QSize m_restoreSize;
	// 移动窗口的变量;
	bool m_isPressed;
	QPoint m_startMovePos;
	// 标题栏跑马灯效果时钟;
	QTimer m_titleRollTimer;
	// 标题栏内容;
	QString m_titleContent;
	// 按钮类型;
	ButtonType m_buttonType;
	// 窗口边框宽度;
	int m_windowBorderWidth;
	// 标题栏是否透明;
	bool m_isTransparent;
	//标题开始坐标(其实就是图标的右侧)
	int m_nPos;

	//当前鼠标是否在标题栏上
	BOOL isTitleUnderMouse;
	//标题是否转换方向

	BOOL m_isChange;
	//鼠标点击次数
	int m_nClickTimes; 
	//单双击计时器
	QTimer m_cTimer; 

	// 保存/获取 最大化前窗口的位置及大小;
	void saveRestoreInfo(const QPoint point, const QSize size);
	void getRestoreInfo(QPoint& point, QSize& size);





private:
	QMenu* m_RightButtonMenu; //右键菜单

	QAction* m_MenuRestore; //还原
	QAction* m_MenuMin; //最小化
	QAction* m_MenuMax; //最大化
	QAction* m_MenuClose; //关闭
	void contextMenuEvent(QContextMenuEvent* event);
};

#endif
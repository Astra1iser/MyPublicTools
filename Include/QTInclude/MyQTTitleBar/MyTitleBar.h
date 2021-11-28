#pragma   once
#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H
#include <QtWidgets>
#include <QWidget>
#include <QtWidgets/QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QtMath>
#include "stdafx.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")


class TitleMonitor;

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

#define BUTTON_HEIGHT 30        // 按钮高度;
#define BUTTON_WIDTH 42         // 按钮宽度;
#define TITLE_HEIGHT 30         // 标题栏高度;

//待开发功能 贴边毛玻璃效果(这个貌似需要封装一个窗体类,不要在标题类上做了)
//焦点问题
//斜角拉伸

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
	//以下设置为必要
	m_titleBar = new MyTitleBar(this);
	m_titleBar->setTitleIcon("../Image/QAXico.png");				//设置图标
	m_titleBar->setTitleContent(QStringLiteral(" 这是标题"),10);	//设置标题和标题大小
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);					//设置需要哪几个功能按钮

	//以下设置可选
	// 设置标题栏背景色及是否设置标题栏背景色透明;
	void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
	// 设置标题栏高度;
	void setTitleHeight(int height = 0);
	// 设置标题栏中的标题是否会滚动(值不要太小,会巨量增加Cpu消耗);
	void setTitleRoll(int timeInterval = 5);
	// 设置向内宽度(设置后,标题栏的左右和上方会空出对应大小的宽度);
	void setWindowBorderWidth(int borderWidth);
	// 设置开启右键菜单(这里只给出了基本功能,还需要什么功能自己去加)
	void setRightClickMenu();
	// 设置窗体是否可拉伸
	void setStretch(BOOL Options = TRUE);

} */


class MyTitleBar : public QDialog
{
	Q_OBJECT

public:
	MyTitleBar(QWidget *parent);
	// 这里parent没有给默认值NULL，保证在创建MyTitleBar对象时父指针必须得赋值;且赋值不为NULL;
	~MyTitleBar();


//以下选项为用户设置
	// 设置标题栏背景色及是否设置标题栏背景色透明;
	void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
	// 设置标题栏图标;
	void setTitleIcon(QString filePath , QSize IconSize = QSize(25 , 25));
	// 设置标题内容;
	void setTitleContent(QString titleContent , int titleFontSize = 9, const char* ColorStyle = "color:white;");
	// 设置标题栏长度(暂时没用);
	void setTitleWidth(int width = 0);
	// 设置标题栏高度;
	void setTitleHeight(int height = 0);
	// 设置标题栏上按钮类型;
	void setButtonType(ButtonType buttonType);
	// 设置标题栏中的标题是否会滚动(值不要太小,会巨量增加Cpu消耗);
	void setTitleRoll(int timeInterval = 5);
	// 设置向内宽度(设置后,标题栏的左右和上方会空出对应大小的宽度);
	void setWindowBorderWidth(int borderWidth);
	// 设置开启右键菜单(这里只给出了基本功能,还需要什么功能自己去加)
	void setRightClickMenu();
	// 设置窗体是否可拉伸
	void setStretch(BOOL Options = TRUE);

//外部用到的函数
public:
	//获取桌面用户区信息(起点坐标和大小)
	QRect getDesktopRect();
	//监控线程发送消息的函数
	void sendMonitormessage();

//外部需要的属性
public:
	BOOL m_CorrectionType;				// 桌面分辨率改变标识符,windows事件发送到此窗口时自动改变为1,处理完数据后置0
	QRect m_PreviousFactRect;			// 客桌面户区变化前的桌面客户区布局
	BOOL m_isStretch;					// 用户设置的窗体是否可拉伸属性
	BOOL m_isMaximize;					// 当前是否是最大化

//内部实现函数
private:
	// 初始化控件;
	void initControl();
	// 信号槽的绑定;
	void initConnections();
	// 加载样式文件;
	void loadStyleSheet(const QString &sheetName);
	// 保存/获取 最大化前窗口的位置及大小;
	void saveRestoreInfo(const QPoint point, const QSize size);
	void getRestoreInfo(QPoint& point, QSize& size);

//内部用到的属性
private:
	QHBoxLayout* mylayout;
	QLabel* m_pIcon;                    // 标题栏图标;
	QLabel* m_pTitleContent;            // 标题栏内容;
	QPushButton* m_pButtonMin;          // 最小化按钮;
	QPushButton* m_pButtonRestore;      // 最大化还原按钮;
	QPushButton* m_pButtonMax;          // 最大化按钮;
	QPushButton* m_pButtonClose;        // 关闭按钮;
	int m_ButtonWidthCount;				// 按钮当前的总宽度

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

	QTimer m_titleRollTimer;			// 标题栏跑马灯效果时钟;
	QString m_titleContent;				// 标题栏内容;
	int m_TitleBarHeight;				// 标题高度;
	bool m_isTransparent;				// 标题栏是否透明;
	int m_nPos;							// 标题开始坐标(其实就是图标的右侧)
	BOOL m_isChange;					// 标题是否需要转换方向

	QTimer m_cTimer;					// 单双击计时器
	int m_nClickTimes;					// 鼠标点击次数
	BOOL m_CursorIsLeftClick;			// 本次是左键点击

	QMenu* m_RightButtonMenu;			//	右键菜单
	QAction* m_MenuRestore;				//	右键菜单还原选项
	QAction* m_MenuMin;					//	右键菜单最小化选项
	QAction* m_MenuMax;					//	右键菜单最大化选项
	QAction* m_MenuClose;				//	右键菜单关闭选项
	BOOL m_isRightClickMenuOn;			//	用户是否开启了右键菜单

	ButtonType m_buttonType;			// 按钮类型;
	int m_windowBorderWidth;			// 窗口边框宽度;
	HWND m_ParentHwnd;					// 父窗体句柄
	BOOL isTitleUnderMouse;				// 当前鼠标是否在标题栏上
	QRect m_CurrentFactRect;			// 保存客桌面户区变化后的(当前的)桌面客户区布局
	TitleMonitor* m_TitleMonitor;		// 监控者线程指针
	BOOL m_isStretch_buffer;			// 用户设置的窗体是否可拉伸属性的内部备份,用于内布切换属性和还原


//重写基类事件函数
private:
	void paintEvent(QPaintEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

//信号
signals:
	void monitorMessage_1();

//槽函数
private slots:
	// 判断单双击的计时器槽函数
	void slotTimerTimeOut();
	// 右键菜单的槽函数
	void slotPlayArgsMenu(const QPoint pos);
	// 最大化后,若窗口所在桌面的分辨率或者客户区布局发生改变后的最大化修正函数
	void CorrectionWindow();
	// 标题滚动槽函数
	void onRollTitle();
	// 按钮触发的槽;
	void onButtonMinClicked();
	void onButtonMinPressed();
	void onButtonMaxClicked();
	void onButtonMaxPressed();
	void onButtonRestoreClicked();
	void onButtonRestorePressed();
	void onButtonCloseClicked();
	void onButtonClosePressed();

};

#endif


//标题监控者线程,在这里,你可以从另一个线程中持续监控标题类中的任何数据,虽然这会消耗一定的性能
class TitleMonitor :public Thread
{
public:

	TitleMonitor(MyTitleBar* mtb)
	{
		this->mtb = mtb;
	}

	// 重写Run;
	void Run() override
	{
		while (this->bRun)
		{
			mtb->sendMonitormessage();
			Sleep(100);
		}
		return;
	}

	MyTitleBar* mtb;
};
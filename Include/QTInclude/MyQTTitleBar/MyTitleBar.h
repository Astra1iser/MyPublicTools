#pragma   once
#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H
#include <QWidget>
#include <QtWidgets/QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

enum ButtonType
{
	MIN_BUTTON = 0,         // 最小化和关闭按钮;
	MIN_MAX_BUTTON ,        // 最小化、最大化和关闭按钮;
	ONLY_CLOSE_BUTTON       // 只有关闭按钮;
};














//如何使用这个自定义标题栏?

//首先需要在窗体中初始化自定义标题栏对象并重写某些函数
/* 
protected:
	MyTitleBar* m_titleBar;
	
private:
	void initTitleBar();									//初始化标题
	
private slots:
	void onButtonMinClicked();								//最小化槽函数
	void onButtonRestoreClicked();							//还原键槽函数
	void onButtonMaxClicked();								//最大化槽函数
	void onButtonCloseClicked();							//关闭按钮槽函数 */

//然后重新实现以下函数
/* void QAXSafeCheckerTrustDlg::onButtonMinClicked()
{
	showMinimized();
}

void QAXSafeCheckerTrustDlg::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	m_titleBar->getRestoreInfo(windowPos, windowSize);
	this->setGeometry(QRect(windowPos, windowSize));
}

void QAXSafeCheckerTrustDlg::onButtonMaxClicked()
{
	m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(FactRect);
}

void QAXSafeCheckerTrustDlg::onButtonCloseClicked()
{
	close();
} 
*/




//然后在主窗体中初始化这个方法(一定要在 ui.setupUi(this); 后)
/* void QAXSafeCheckerTrustDlg ::initTitleBar()
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	
	// 设置窗口背景透明;
	this->setAttribute(Qt::WA_TranslucentBackground);


	m_titleBar = new MyTitleBar(this);
	m_titleBar->move(0, 0);
	m_titleBar->setTitleIcon("../Image/QAXico.png");
	m_titleBar->setTitleContent(QStringLiteral(" 这是标题"),10);
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());


	connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
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
	void setTitleContent(QString titleContent , int titleFontSize = 9);
	// 设置标题栏长度;
	void setTitleWidth(int width);
	// 设置标题栏上按钮类型;
	void setButtonType(ButtonType buttonType);
	// 设置标题栏中的标题是否会滚动;具体可以看效果;
	void setTitleRoll();
	// 设置窗口边框宽度;
	void setWindowBorderWidth(int borderWidth);

	// 保存/获取 最大化前窗口的位置及大小;
	void saveRestoreInfo(const QPoint point, const QSize size);
	void getRestoreInfo(QPoint& point, QSize& size);

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
	// 按钮触发的信号;
	void signalButtonMinClicked();
	void signalButtonRestoreClicked();
	void signalButtonMaxClicked();
	void signalButtonCloseClicked();

	private slots:
		// 按钮触发的槽;
		void onButtonMinClicked();
		void onButtonRestoreClicked();
		void onButtonMaxClicked();
		void onButtonCloseClicked();
		void onRollTitle();

private:
	QLabel* m_pIcon;                    // 标题栏图标;
	QLabel* m_pTitleContent;            // 标题栏内容;
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
};

#endif
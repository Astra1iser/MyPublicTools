#include "MyTitleBar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>

#define BUTTON_HEIGHT 30        // 按钮高度;
#define BUTTON_WIDTH 30         // 按钮宽度;
#define TITLE_HEIGHT 30         // 标题栏高度;

MyTitleBar::MyTitleBar(QWidget *parent)
	: QDialog(parent)
	, m_colorR(0)
	, m_colorG(120)
	, m_colorB(232)
	, m_isPressed(false)
	, m_buttonType(MIN_MAX_BUTTON)
	, m_windowBorderWidth(0)
	, m_isTransparent(false)
	, m_nPos(25)
	, m_isChange(0)
	, m_CanMove(0)
	//, m_height(30)
{
	// 初始化;
	initControl();
	initConnections();
	// 加载本地样式 MyTitle.css文件;
	//loadStyleSheet(QString::fromLocal8Bit("信任区"));
}

MyTitleBar::~MyTitleBar()
{

}

// 初始化控件;
void MyTitleBar::initControl()
{
	m_pIcon = new QLabel;
	m_pTitleContent = new QLabel;

	m_pButtonMin = new QPushButton;			//最小化按钮
	m_pButtonRestore = new QPushButton;		//还原按钮
	m_pButtonMax = new QPushButton;			//最大化按钮
	m_pButtonClose = new QPushButton;		//关闭按钮

	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	m_pButtonMin->setToolTip(QStringLiteral("最小化"));
	m_pButtonRestore->setToolTip(QStringLiteral("还原"));
	m_pButtonMax->setToolTip(QStringLiteral("最大化"));
	m_pButtonClose->setToolTip(QStringLiteral("关闭"));

	QHBoxLayout* mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);


	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);
	m_pButtonMin->setStyleSheet("QPushButton{border: transparent; border-radius: none; background-color:none;}"
		"QPushButton{border-image:url(image/Minimize.png) 0 84 0 0;}"
		"QPushButton:hover{border-image:url(image/Minimize.png) 0 42 0 42;}"
		"QPushButton:pressed{border-image:url(image/Minimize.png) 0 0 0 84;}");

	m_pButtonRestore->setStyleSheet("QPushButton{border: transparent; border-radius: none; background-color:none;}"
		"QPushButton{border-image:url(image/Restore.png) 0 84 0 0;}"
		"QPushButton:hover{border-image:url(image/Restore.png) 0 42 0 42;}"
		"QPushButton:pressed{border-image:url(image/Restore.png) 0 0 0 84;}");

	m_pButtonMax->setStyleSheet("QPushButton{border: transparent; border-radius: none; background-color:none;}"
		"QPushButton{border-image:url(image/Maximize.png) 0 84 0 0;}"
		"QPushButton:hover{border-image:url(image/Maximize.png) 0 42 0 42;}"
		"QPushButton:pressed{border-image:url(image/Maximize.png) 0 0 0 84;}");

	m_pButtonClose->setStyleSheet("QPushButton{border: transparent; border-radius: none; background-color:none;}"
		"QPushButton{border-image:url(image/Close.png) 0 84 0 0;}"
		"QPushButton:hover{border-image:url(image/Close.png) 0 42 0 42;}"
		"QPushButton:pressed{border-image:url(image/Close.png) 0 0 0 84;}");

	mylayout->setContentsMargins(5, 0, 0, 0);
	mylayout->setSpacing(0);


	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


	//设置父窗体图标点击,外框隐藏
	this->parentWidget()->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	//this->setWindowFlags(Qt::FramelessWindowHint);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	this->setTitleWidth();
	this->setTitleHeight();
}

// 信号槽的绑定;
void MyTitleBar::initConnections()
{
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));


	connect(this, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked2()));
	connect(this, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked2()));
	connect(this, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked2()));
	connect(this, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked2()));
}

// 设置标题栏背景色,在paintEvent事件中进行绘制标题栏背景色;
// 在构造函数中给了默认值，可以外部设置颜色值改变标题栏背景色;
void MyTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
	m_colorR = r;
	m_colorG = g;
	m_colorB = b;
	m_isTransparent = isTransparent;
	// 重新绘制（调用paintEvent事件）;
	update();
}

// 设置标题栏图标;
void MyTitleBar::setTitleIcon(QString filePath, QSize IconSize)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setPixmap(titleIcon.scaled(IconSize));
	m_nPos = m_pIcon->width();
}

// 设置标题内容;
void MyTitleBar::setTitleContent(QString titleContent, int titleFontSize, const char* ColorStyle)
{
	// 设置标题字体大小;
	QFont font = m_pTitleContent->font();
	font.setBold(true);
	font.setPointSize(titleFontSize);
	m_pTitleContent->setFont(font);
	//m_pTitleContent->setStyleSheet("color:white;"); 
	m_pTitleContent->setStyleSheet(ColorStyle); 
	// 设置标题内容;
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

// 设置标题栏长度;(这个函数目前没用,默认标题栏都是充满顶部的,除非使用了setWindowBorderWidth()设置了父窗体中边框线的宽度)
void MyTitleBar::setTitleWidth(int width)
{
	if (0 == width)
	{
		width = this->parentWidget()->width();
	}
	this->setFixedWidth(width);

}

// 设置标题栏高度(最少为30,低于30的一律认为是30,因为太低了会变形不好看,有需求的自己改这里的代码);
void MyTitleBar::setTitleHeight(int height)
{
	if (0 == height || TITLE_HEIGHT > height)
	{
		height = TITLE_HEIGHT;
	}
	this->setFixedHeight(height);
	//m_height = this->height();
}


// 设置标题栏上按钮类型;
// 由于不同窗口标题栏上的按钮都不一样，所以可以自定义标题栏中的按钮;
// 这里提供了四个按钮，分别为最小化、还原、最大化、关闭按钮，如果需要其他按钮可自行添加设置;
void MyTitleBar::setButtonType(ButtonType buttonType)
{
	m_buttonType = buttonType;

	switch (buttonType)
	{
	case MIN_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
	case MIN_MAX_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
		}
		break;
	case ONLY_CLOSE_BUTTON:
		{
			m_pButtonMin->setVisible(false);
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
	default:
		break;
	}
}

// 设置标题栏中的标题是否会自动滚动，跑马灯的效果;
// 一般情况下标题栏中的标题内容是不滚动的
void MyTitleBar::setTitleRoll(int timeInterval)
{
	connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
	m_titleRollTimer.start(timeInterval);
}

// 设置窗口边框宽度;
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
	m_windowBorderWidth = borderWidth;
}

// 保存窗口最大化前窗口的位置以及大小;
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

// 获取窗口最大化前窗口的位置以及大小;
void MyTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}

// 绘制标题栏背景色;
void MyTitleBar::paintEvent(QPaintEvent *event)
{
	// 是否设置标题透明;
	if (!m_isTransparent)
	{
		//设置背景色;
		QPainter painter(this);
		QPainterPath pathBack;
		pathBack.setFillRule(Qt::WindingFill);
		//在左上方和右上方设置圆角 这里直接设置0,有需要的自己改
		pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 0, 0);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
	}   

	// 当窗口最大化或者还原后，窗口长度变了，标题栏的长度应当一起改变;
	// 这里减去m_windowBorderWidth ，是因为窗口可能设置了不同宽度的边框;
	// 如果窗口有边框则需要设置m_windowBorderWidth的值，否则m_windowBorderWidth默认为0;
	if (this->width() != (this->parentWidget()->width() - 2*m_windowBorderWidth))
	{
		this->setFixedWidth(this->parentWidget()->width() - 2*m_windowBorderWidth);
	}
	//if (m_height - m_windowBorderWidth < 30)
	//判断高度是否正常,这里其实根本不用判断,但是为了对应宽度判断,还是加上吧
	if (this->height() < TITLE_HEIGHT)
	{
		this->setFixedHeight(TITLE_HEIGHT);
	}
	
	//设置了边框时左右和上方要腾出对应大小的边框位置
	this->move(m_windowBorderWidth, m_windowBorderWidth);
	QWidget::paintEvent(event);
}

// 双击响应事件，主要是实现双击标题栏进行最大化和最小化操作;
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	// 只有存在最大化、还原按钮时双击才有效;
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		// 通过最大化按钮的状态判断当前窗口是处于最大化还是原始大小状态;
		// 或者通过单独设置变量来表示当前窗口状态;
		if (m_pButtonMax->isVisible())
		{
			onButtonMaxClicked();
		}
		else
		{
			onButtonRestoreClicked();
		}
	}

	return QWidget::mouseDoubleClickEvent(event);
}

// 以下通过mousePressEvent、mouseMoveEvent、mouseReleaseEvent三个事件实现了鼠标拖动标题栏移动窗口的效果;
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
	//if (m_buttonType == MIN_MAX_BUTTON)
	//{
	//	if (m_pButtonMax->isVisible())
	//	{
	//		m_isPressed = true;
	//		m_startMovePos = event->globalPos();
	//	}
	//}
	//else
	//{
	//	m_isPressed = true;
	//	m_startMovePos = event->globalPos();
	//}

	//bool dd = this->geometry().contains(this->mapFromGlobal(QCursor::pos()));

	m_isPressed = true;
	m_startMovePos = event->globalPos();

	
	return QWidget::mousePressEvent(event);
}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{	
	//如果当前恢复按钮可用,证明点击过最大化或者双击过标题栏
	//if (m_pButtonRestore->isVisible() && !m_pButtonMax->isVisible() && this->geometry().contains(this->mapFromGlobal(QCursor::pos()))/*&& this->parentWidget()->windowState() == Qt::WindowMaximized*/ )
	//{
	//	//让当前的窗体标题终点移动到鼠标坐标上
	//	m_CanMove = 1;
	//}
	//else
	//{
	//	//不让当前的窗体标题终点移动到鼠标坐标上
	//	m_CanMove = 0;
	//}

	//如果鼠标移动时,发现当前窗体的恢复按钮存在,且可以拖动则触发还原逻辑(仿微软设计)
	if (m_pButtonRestore->isVisible() && this->geometry().contains(this->mapFromGlobal(QCursor::pos())) /* && m_CanMove == 1*/)
	{
		//本设计是还原微软窗体的动作,最大化移动窗体时,还原原始窗体并跟随鼠标移动
		//触发一次窗体还原
		onButtonRestoreClicked();
		//m_CanMove = 0;
		QPoint movePoint = event->globalPos();
		//让父窗口的标题中间对准鼠标
		//this->parentWidget()->move(movePoint.x()- this->parentWidget()->width()/2,  movePoint.y()- this->height()/2);
		this->parentWidget()->move(movePoint.x() - this->parentWidget()->width() / 2, movePoint.y() - m_windowBorderWidth - this->height() / 2);
	}

	if (m_isPressed && this->geometry().contains(this->mapFromGlobal(QCursor::pos())))
	{
		QPoint movePoint = event->globalPos() - m_startMovePos;
		QPoint widgetPos = this->parentWidget()->pos();
		m_startMovePos = event->globalPos();
		this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
	}


	return QWidget::mouseMoveEvent(event);
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

// 加载本地样式文件;
// 可以将样式直接写在文件中，程序运行时直接加载进来;
void MyTitleBar::loadStyleSheet(const QString &sheetName)
{
	QFile file(":/Resources/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
}

// 以下为按钮操作响应的槽;
void MyTitleBar::onButtonMinClicked()
{
	emit signalButtonMinClicked();
}

void MyTitleBar::onButtonRestoreClicked()
{
	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

void MyTitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
	emit signalButtonCloseClicked();
}

// 该方法主要是让标题栏中的标题显示为滚动的效果;
void MyTitleBar::onRollTitle()
{
	//static int nPos = 0;
	//QString titleContent = m_titleContent;
	//// 当截取的位置比字符串长时，从头开始;
	//if (nPos > titleContent.length())
	//	nPos = 0;

	//m_pTitleContent->setText(titleContent.mid(nPos));
	//nPos++;


	QString titleContent = m_titleContent;
	QFontMetrics fm(m_pTitleContent->font());
	int textWidthInPxs = fm.width(m_titleContent);

	int ButtonWidthCount = m_pButtonMin->width() + m_pButtonMax->width() + m_pButtonRestore->width() + m_pButtonClose->width();

	if ((m_nPos + textWidthInPxs) >= (this->width() - ButtonWidthCount))
	{
		m_nPos = this->width()- ButtonWidthCount - textWidthInPxs;
		m_isChange = TRUE;
	}

	if (m_nPos <= m_pIcon->width())
	{
		m_nPos = m_pIcon->width();
		m_isChange = FALSE;
	}

	m_pTitleContent->move(m_nPos, (m_pTitleContent->pos().y()));

	if (FALSE == m_isChange)
		m_nPos++;
	else
		m_nPos--;


}



// 以下为按钮操作响应的槽;
void MyTitleBar::onButtonMinClicked2()
{
	this->parentWidget()->showMinimized();
}

void MyTitleBar::onButtonRestoreClicked2()
{
	this->parentWidget()->setWindowState(Qt::WindowNoState);
	//原始窗口可以扩展大小(仿windows设计)
	this->parentWidget()->setMaximumHeight(16777215);
	this->parentWidget()->setMaximumWidth(16777215);
	this->parentWidget()->setMinimumHeight(0);
	this->parentWidget()->setMinimumWidth(0);

	QPoint windowPos;
	QSize windowSize;
	//把之前保存的原始窗体大小取出来还原
	this->getRestoreInfo(windowPos, windowSize);
	this->parentWidget()->setGeometry(QRect(windowPos, windowSize));
}

void MyTitleBar::onButtonMaxClicked2()
{
	//保存一下原始窗体大小,便于还原时用
	this->saveRestoreInfo(this->parentWidget()->pos(), QSize(this->parentWidget()->width(), this->parentWidget()->height()));

	//此处为了兼容多屏幕,判断当前进程所在屏幕的序号
	QDesktopWidget* deskTop = QApplication::desktop();
	int curMonitor = deskTop->screenNumber(this); // 参数是一个QWidget*
	//取所在屏幕桌面的客户大小(去掉任务栏后的大小)
	QRect desktopRect = QApplication::desktop()->availableGeometry(curMonitor);
	QRect FactRect = QRect(desktopRect.x(), desktopRect.y(), desktopRect.width(), desktopRect.height());

	//从当前进程屏幕上左上角位置开始，显示一个当前桌面客户分辨率大小(此分辨率比桌面分辨率小,因为去掉了任务栏的大小)的界面（宽desktopRect.width()，高esktopRect.height().
	this->parentWidget()->setGeometry(FactRect);
	//this->parentWidget()->setWindowState(Qt::WindowMaximized);

	//最大化时禁止拉伸窗口(仿windows设计)
	this->parentWidget()->setMinimumHeight(this->parentWidget()->height());
	this->parentWidget()->setMinimumWidth(this->parentWidget()->width());
	this->parentWidget()->setMaximumHeight(this->parentWidget()->height());
	this->parentWidget()->setMaximumWidth(this->parentWidget()->width());
}

void MyTitleBar::onButtonCloseClicked2()
{
	this->parentWidget()->close();
}

#include "MyTitleBar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>

MyTitleBar::MyTitleBar(QWidget* parent)
	: QDialog(parent)
	, m_colorR(0)
	, m_colorG(120)
	, m_colorB(232)
	, m_isPressed(false)
	, m_buttonType(MIN_MAX_BUTTON)
	, m_windowBorderWidth(0)
	, m_isTransparent(false)
	, m_nPos(30) //30是由25+5构成的 // 图标宽度是25,+5是因为布局时设置了5的左边距
	, m_isChange(0)
	, m_nClickTimes(0)
	, m_isRightClickMenuOn(0)
	, m_TitleBarHeight(TITLE_HEIGHT)
	, m_CorrectionType(0)
	, m_TitleMonitor(new TitleMonitor(this))
	, m_isStretch(TRUE)
	, m_isMaximize(FALSE)
{
	// 初始化;
	initControl();
	initConnections();
	// 加载本地样式 MyTitle.css文件;
	//loadStyleSheet(QString::fromLocal8Bit(""));
	// 监控线程启动
	m_TitleMonitor->Start();
}

MyTitleBar::~MyTitleBar()
{
	//以下指针指向的区域为必定申请的
	delete m_frame;
	delete m_TitleMonitor;
	delete m_pIcon;
	delete m_pTitleContent;
	delete m_pButtonMin;
	delete m_pButtonRestore;
	delete m_pButtonMax;
	delete m_pButtonClose;
	delete mylayout;

	//以下指针可能申请了空间
	if (m_isRightClickMenuOn)
	{
		delete m_RightButtonMenu;
		delete m_MenuRestore;
		delete m_MenuMin;
		delete m_MenuMax;
		delete m_MenuClose;
	}
}

// 初始化控件;
void MyTitleBar::initControl()
{
	m_frame = new MyFrame(this->parentWidget());	//边框

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

	mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	//如何设置QSS https://blog.csdn.net/Andy_93/article/details/53100640?utm_source=blogxgwz9
	//这里图标是我自己做的,如果有别的外观要求,我暂时没有提供外部方法,所以请改这里,其实也可以把这三个按钮放在public里,然后外部修改

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
	// 让标题栏透明,不要遮盖下方的标题颜色
	m_pTitleContent->setAttribute(Qt::WA_TranslucentBackground, true);

	//设置父窗体图标点击,外框隐藏,如果父窗体有需求,一定要改这里或者重新给父窗体设置属性
	this->parentWidget()->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	/*
	//此处代码可以实现标题栏和边框,在nativeEvent()会再次去掉标题栏
	//主要的关键是WS_THICKFRAME
	*/
	HWND m_ParentHwnd = (HWND)this->parentWidget()->winId();
	DWORD style = ::GetWindowLong(m_ParentHwnd, GWL_STYLE);
	::SetWindowLong(m_ParentHwnd, GWL_STYLE, style | WS_THICKFRAME);
	//保留一个像素的边框宽度，否则系统不会绘制边框阴影
	const MARGINS shadow = { 1, 1, 1, 1 };
	//绘制平面阴影win10以下不支持
	DwmExtendFrameIntoClientArea(m_ParentHwnd, &shadow);
	//绘制立体阴影全系统支持
	SetClassLong(m_ParentHwnd, GCL_STYLE, GetClassLong(m_ParentHwnd, GCL_STYLE) | CS_DROPSHADOW);//窗体阴影

	//BOOL bEnable = false;
	//::DwmIsCompositionEnabled(&bEnable);

	//if (bEnable)
	//{
	//	DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
	//	::DwmSetWindowAttribute(m_ParentHwnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
	//	MARGINS margins = { -1 };
	//	//绘制平面阴影win10以下不支持
	//	::DwmExtendFrameIntoClientArea(m_ParentHwnd, &margins);
	//}

	//this->setWindowFlags(Qt::FramelessWindowHint);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	this->setTitleWidth();
	this->setTitleHeight();
}

//设置(更新)父窗体边框
void  MyTitleBar::setFrame()
{
	if (m_isMaximize)
	{	
		m_frame->m_Top_Horizontal_Line->setVisible(FALSE);
		m_frame->m_Bottom_Horizontal_Line->setVisible(FALSE);
		m_frame->m_Left_Vertical_Line->setVisible(FALSE);
		m_frame->m_Right_Vertical_Line->setVisible(FALSE);
		return;
	}
	else
	{
		m_frame->m_Top_Horizontal_Line->setVisible(TRUE);
		m_frame->m_Bottom_Horizontal_Line->setVisible(TRUE);
		m_frame->m_Left_Vertical_Line->setVisible(TRUE);
		m_frame->m_Right_Vertical_Line->setVisible(TRUE);
	}

	if (m_windowBorderWidth != 0)
	{
		//上边界
		m_frame->m_Top_Horizontal_Line->setGeometry(QRect(-1, 0, this->parentWidget()->width()+2, 2));
		//下边界
		m_frame->m_Bottom_Horizontal_Line->setGeometry(QRect(0, this->parentWidget()->height()-1, this->parentWidget()->width()+2, 2));
		//左边界
		m_frame->m_Left_Vertical_Line->setGeometry(QRect(0, 0, 2, this->parentWidget()->height()));
		//右边界
		m_frame->m_Right_Vertical_Line->setGeometry(QRect(this->parentWidget()->width()-1, -1, 2, this->parentWidget()->height()+2));
		return;
	}
	else
	{	
		//上边界
		m_frame->m_Top_Horizontal_Line->setGeometry(QRect(-1, 0, this->parentWidget()->width()+2, 0));
		//下边界
		m_frame->m_Bottom_Horizontal_Line->setGeometry(QRect(0, this->parentWidget()->height() - 1, this->parentWidget()->width()+2, 2));
		//左边界
		m_frame->m_Left_Vertical_Line->setGeometry(QRect(0, m_TitleBarHeight, 2, this->parentWidget()->height()- m_TitleBarHeight));
		//右边界
		m_frame->m_Right_Vertical_Line->setGeometry(QRect(this->parentWidget()->width() - 1, -1, 2, this->parentWidget()->height() + 2));
		return;
	}
}

// 信号槽的绑定;
void MyTitleBar::initConnections()
{
	//注意这个定时器绑定是用来判断单双击的,一定不要删除
	connect(&m_cTimer, SIGNAL(timeout()), this, SLOT(slotTimerTimeOut()));

	//以下是按钮的信号槽
	connect(m_pButtonMin, SIGNAL(pressed()), this, SLOT(onButtonMinPressed()));
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonMax, SIGNAL(pressed()), this, SLOT(onButtonMaxPressed()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonRestore, SIGNAL(pressed()), this, SLOT(onButtonRestorePressed()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonClose, SIGNAL(pressed()), this, SLOT(onButtonClosePressed()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));

	//以下是监控者线程信号槽,可以自定义扩充
	connect(this, SIGNAL(monitorMessage_1()), this, SLOT(CorrectionWindow()));
}








//用户设置函数区

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
	//m_nPos = m_pIcon->width();
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
		m_TitleBarHeight = TITLE_HEIGHT;
	}
	else
	{
		m_TitleBarHeight = height;
	}
	this->setFixedHeight(m_TitleBarHeight);
}

// 设置窗口边框宽度;
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
	m_windowBorderWidth = borderWidth;
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
			m_ButtonWidthCount = m_pButtonMin->width() + m_pButtonClose->width();
		}
		break;
	case MIN_MAX_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
			m_ButtonWidthCount = m_pButtonMin->width() + m_pButtonMax->width() + m_pButtonClose->width();
		}
		break;
	case ONLY_CLOSE_BUTTON:
		{
			m_pButtonMin->setVisible(false);
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
			m_ButtonWidthCount = m_pButtonClose->width();
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
	m_nPos = m_pIcon->width() + 5; //设置滚动起始位置 +5是因为布局时设置了5的左边距
	connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
	m_titleRollTimer.start(timeInterval);
}

//设置右键菜单
void MyTitleBar:: setRightClickMenu()
{
	//参考 https://blog.csdn.net/qq_33266987/article/details/54017072?utm_medium=distribute.pc_feed_404.none-task-blog-2~default~BlogCommendFromBaidu~default-8.test_5searchdownload&depth_1-utm_source=distribute.pc_feed_404.none-task-blog-2~default~BlogCommendFromBaidu~default-8.test_5searchdownloa
	
	if (m_isRightClickMenuOn)
	{
		return;
	}

	m_isRightClickMenuOn = TRUE;
	this->setContextMenuPolicy(Qt::CustomContextMenu);//添加右键菜单策略
	m_RightButtonMenu = new QMenu(this); //菜单实体的指针

	//选项
	m_MenuRestore = new QAction(QIcon("image/restoreBlack_16.png"), YString("还原"), this); //还原
	m_MenuMin = new QAction(QIcon("image/minimizeBtnBlack_16.png"), YString("最小化"), this); //最小化
	m_MenuMax = new QAction(QIcon("image/maximizeBtnBlack_16.png"), YString("最大化"), this); //最大化
	m_MenuClose = new QAction(QIcon("image/closeBtnBlack_16.png"), YString("关闭"), this); //关闭

	if (MIN_BUTTON == m_buttonType)
	{
		connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPlayArgsMenu(QPoint)));
		connect(m_MenuMin, SIGNAL(triggered()), this, SLOT(onButtonMinClicked()));
		connect(m_MenuClose, SIGNAL(triggered()), this, SLOT(onButtonCloseClicked()));

		return;
	}

	if (MIN_MAX_BUTTON == m_buttonType)
	{
		connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPlayArgsMenu(QPoint)));
		connect(m_MenuRestore, SIGNAL(triggered()), this, SLOT(onButtonRestoreClicked()));
		connect(m_MenuMin, SIGNAL(triggered()), this, SLOT(onButtonMinClicked()));
		connect(m_MenuMax, SIGNAL(triggered()), this, SLOT(onButtonMaxClicked()));
		connect(m_MenuClose, SIGNAL(triggered()), this, SLOT(onButtonCloseClicked()));
	}

	if (ONLY_CLOSE_BUTTON == m_buttonType)
	{
		connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPlayArgsMenu(QPoint)));
		connect(m_MenuClose, SIGNAL(triggered()), this, SLOT(onButtonCloseClicked()));
	}
	
}

// 设置当前窗体是否可拉伸
void MyTitleBar::setStretch(BOOL Options)
{
	m_isStretch = Options;
}







//事件区

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
		//别让标题栏大小成负数,不然按QT的处理特性会变成人工智障(让标题栏消失)
		if (this->parentWidget()->width() - 2 * m_windowBorderWidth > 0)
		{
			this->setFixedWidth(this->parentWidget()->width() - 2 * m_windowBorderWidth);
		}
	}

	//判断高度是否正常,这里其实根本不用判断,但是为了对应宽度判断,还是加上吧
	if (this->height() < TITLE_HEIGHT)
	{
		this->setFixedHeight(TITLE_HEIGHT);
	}
	//设置了边框时左右和上方要腾出对应大小的边框位置
	this->move(m_windowBorderWidth, m_windowBorderWidth);
	setFrame();
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
		if (m_pButtonMax->isVisible() && !this->m_pIcon->underMouse())
		{
			if (event->button() == Qt::LeftButton && DOUBLECLICK == m_nClickTimes+1)//这里加1是因为双击事件发生在单击前,此时m_nClickTimes还没被加1
			{
				onButtonMaxClicked();
				return QWidget::mouseDoubleClickEvent(event);
			}
		}
		else if (m_pButtonRestore->isVisible() && !this->m_pIcon->underMouse())
		{
			if (event->button() == Qt::LeftButton && DOUBLECLICK == m_nClickTimes + 1)
			{
				onButtonRestoreClicked();
				return QWidget::mouseDoubleClickEvent(event);
			}
		}
	}

	return QWidget::mouseDoubleClickEvent(event);
}

// 以下通过mousePressEvent、mouseMoveEvent、mouseReleaseEvent三个事件实现了鼠标拖动标题栏移动窗口的效果;
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
	//点击计数器,每点一下加一,用于判断单击双击用
	m_nClickTimes++;
	//单双击计时器,详情请看slotTimerTimeOut()
	m_cTimer.start(300);

	if (event->button() == Qt::LeftButton && m_nClickTimes == 1)
	{
		m_CursorIsLeftClick = TRUE;
		if (this->m_pIcon->underMouse())
		{
			isTitleUnderMouse = FALSE;
			m_isPressed = FALSE;
			emit customContextMenuRequested(mapToGlobal(this->pos()) + QPoint(0-m_windowBorderWidth, m_TitleBarHeight-m_windowBorderWidth));
			return QWidget::mousePressEvent(event);
		}
		else
		{
			isTitleUnderMouse = !this->m_pButtonMin->underMouse()
				&& !this->m_pButtonMax->underMouse()
				&& !this->m_pButtonRestore->underMouse()
				&& !this->m_pButtonClose->underMouse();
			m_isPressed = true;
			m_startMovePos = event->globalPos();
			return QWidget::mousePressEvent(event);
		}
	}

	m_CursorIsLeftClick = FALSE;
	isTitleUnderMouse = FALSE;
	m_isPressed = FALSE;

	return QWidget::mousePressEvent(event);
}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{	
	//如果鼠标移动时,发现当前窗体的恢复按钮存在,且可以拖动则触发还原逻辑(仿微软设计)
	if (m_pButtonRestore->isVisible() && isTitleUnderMouse  /* && m_CanMove == 1*/)
	{
		//本设计是还原微软窗体的动作,最大化移动窗体时,还原原始窗体并跟随鼠标移动
		//触发一次窗体还原
		onButtonRestoreClicked();
		//m_CanMove = 0;
		QPoint movePoint = event->globalPos();
		//让父窗口的标题中间对准鼠标
		this->parentWidget()->move(movePoint.x() - this->parentWidget()->width() / 2, movePoint.y() - m_windowBorderWidth - this->height() / 2);
	}

	if (m_isPressed && isTitleUnderMouse)
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
	if (this->m_pIcon->underMouse() && m_CursorIsLeftClick)
	{
		isTitleUnderMouse = FALSE;
		m_isPressed = FALSE;
		emit customContextMenuRequested(mapToGlobal(this->pos()) + QPoint(0-m_windowBorderWidth, m_TitleBarHeight-m_windowBorderWidth));
		return QWidget::mouseReleaseEvent(event);
	}

	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}








//槽函数区

//单双机计时器槽函数
void MyTitleBar::slotTimerTimeOut()
{
	/*
	* https://blog.csdn.net/beibeix2015/article/details/77488910
	可以用 if (event->button() == Qt::LeftButton && m_nClickTimes == 1)
	来判断鼠标左键单击,左键双击,左键多击,右键单击,右键双击,右键多击,具体怎么用见机行事
	计时器停止, 结算计时器时间内的点击次数, 当点击次数为1时认为单位时间内点击了一次, 点击次数为2时认为单位时间内点击了两次
	注意计时器的时间不要过短,不然对于窗体来说,太短会造成计数器总为1,在mousePressEvent()内判断单击时会一直成功,导致双击时触发单击
	计时器的时间也不能太长,不然对于窗体来说,单击条件为判断点击数1时,你的下一次判断成功的单击延迟会被加长
	这里推荐计时器时间设置为200-400内最好
	*/
	m_cTimer.stop();
	//if (1 == m_nClickTimes)
	//{
	//	//单击 TODU
	//	qDebug() < <YString("单击") << endl;
	//}
	//else if (2 == m_nClickTimes)
	//{
	//	//双击 TODU
	//	qDebug() << YString("双击") << endl;
	//}

	//将计数器归零
	m_nClickTimes = 0;

}

//右键菜单的槽函数
void MyTitleBar::slotPlayArgsMenu(const QPoint pos)
{
	if (m_isRightClickMenuOn)
	{
		BOOL isAllowdisplayarea = !this->m_pButtonMin->underMouse()
			&& !this->m_pButtonMax->underMouse()
			&& !this->m_pButtonRestore->underMouse()
			&& !this->m_pButtonClose->underMouse();

		if (isAllowdisplayarea)
		{
			if (MIN_BUTTON == m_buttonType)
			{
				m_RightButtonMenu->clear();
				m_RightButtonMenu->addAction(m_MenuMin);
				m_RightButtonMenu->addSeparator();    //分割线
				m_RightButtonMenu->addAction(m_MenuClose);
			}

			if (MIN_MAX_BUTTON == m_buttonType)
			{
				m_RightButtonMenu->clear();
				m_RightButtonMenu->addAction(m_MenuRestore);
				m_RightButtonMenu->addAction(m_MenuMin);
				m_RightButtonMenu->addAction(m_MenuMax);
				m_RightButtonMenu->addSeparator();    //分割线
				m_RightButtonMenu->addAction(m_MenuClose);
			}

			if (ONLY_CLOSE_BUTTON == m_buttonType)
			{
				m_RightButtonMenu->clear();
				m_RightButtonMenu->addAction(m_MenuClose);
			}
			
			if (TRUE == m_pButtonMax->isVisible())
			{
				m_MenuRestore->setEnabled(FALSE);
			}
			else
			{
				m_MenuRestore->setEnabled(TRUE);
			}
			if (TRUE == m_pButtonRestore->isVisible())
			{
				m_MenuMax->setEnabled(FALSE);
			}
			else
			{
				m_MenuMax->setEnabled(TRUE);
			}

			if (!this->m_pIcon->underMouse())
				m_RightButtonMenu->exec(QCursor::pos());  //在当前鼠标处堵住
			else
			{
				if (m_CursorIsLeftClick)
					m_RightButtonMenu->exec(pos);  //在特定坐标处堵住
				else
					m_RightButtonMenu->exec(QCursor::pos());  //在当前鼠标处堵住
			}

			isTitleUnderMouse = this->m_pIcon->underMouse()
				&& !this->m_pButtonMin->underMouse()
				&& !this->m_pButtonMax->underMouse()
				&& !this->m_pButtonRestore->underMouse()
				&& !this->m_pButtonClose->underMouse();
		}
	}
}

// 以下为按钮操作响应的槽;
//最小化响应函数,点击时
void MyTitleBar::onButtonMinPressed()
{
	isTitleUnderMouse = FALSE;
}

//最小化响应函数,点击后
void MyTitleBar::onButtonMinClicked()
{
	this->parentWidget()->showMinimized();
}

//最大化响应函数,点击时
void MyTitleBar::onButtonMaxPressed()
{
	isTitleUnderMouse = FALSE;
}

//最大化响应函数,点击后
void MyTitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);

	//保存一下用户设置的拉伸属性,windows的逻辑是最大化禁止拉伸,这里在最大化时也临时设置不能拉伸
	m_isStretch_buffer = m_isStretch;
	m_isStretch = FALSE;

	//保存一下原始窗体大小,便于还原时用
	this->saveRestoreInfo(this->parentWidget()->pos(), QSize(this->parentWidget()->width(), this->parentWidget()->height()));

	QRect desktopRect = getDesktopRect();
	//从当前进程屏幕上左上角位置开始，显示一个当前桌面客户分辨率大小(此分辨率比桌面分辨率小,因为去掉了任务栏的大小)的界面（宽desktopRect.width()，高esktopRect.height().
	this->parentWidget()->setGeometry(desktopRect);

	m_isMaximize = TRUE;
	m_pButtonRestore->setVisible(true);
}

//还原响应函数,点击时
void MyTitleBar::onButtonRestorePressed()
{
	isTitleUnderMouse = FALSE;
}

//还原响应函数,点击后
void MyTitleBar::onButtonRestoreClicked()
{
	m_pButtonRestore->setVisible(false);

	//让拉伸选项还原
	m_isStretch = m_isStretch_buffer;

	QPoint windowPos;
	QSize windowSize;
	//把之前保存的原始窗体大小取出来还原
	this->getRestoreInfo(windowPos, windowSize);
	this->parentWidget()->setGeometry(QRect(windowPos, windowSize));
	//this->parentWidget()->setWindowState(Qt::WindowActive);

	m_isMaximize = FALSE;
	m_pButtonMax->setVisible(true);
}

//关闭响应函数,点击时
void MyTitleBar::onButtonClosePressed()
{
	isTitleUnderMouse = FALSE;
}

//关闭响应函数,点击后
void MyTitleBar::onButtonCloseClicked()
{
	this->parentWidget()->close();
}

// 标题滚动槽函数,该方法主要是让标题栏中的标题显示为滚动的效果;
void MyTitleBar::onRollTitle()
{
	QFontMetrics fm(m_pTitleContent->font());
	int textWidthInPxs = fm.width(m_titleContent);

	if ((m_nPos + textWidthInPxs) >= (this->width() - m_ButtonWidthCount))
	{
		m_nPos = this->width() - m_ButtonWidthCount - textWidthInPxs;
		m_isChange = TRUE;
	}

	if (m_nPos <= m_pIcon->width() + 5)	// +5是因为布局时设置了5的左边距
	{
		m_nPos = m_pIcon->width() + 5;	// +5是因为布局时设置了5的左边距
		m_isChange = FALSE;
	}

	m_pTitleContent->move(m_nPos, (m_pTitleContent->pos().y()));

	if (FALSE == m_isChange)
		m_nPos++;
	else
		m_nPos--;
}

//最大化后,若窗口所在桌面的分辨率或者客户区布局发生改变后的最大化修正槽函数
void MyTitleBar::CorrectionWindow()
{
	//这里设置标志的原因是进行了性能优化,不然一直刷新桌面信息性能消耗过大
	//if (m_CorrectionType == TRUE && m_isMaximize)
	//if (m_isMaximize)
	//{
		//m_CurrentFactRect = getDesktopRect();

		//if (m_CurrentFactRect == m_PreviousFactRect)
		//{
		//	//从当前进程屏幕上左上角位置开始，显示一个当前桌面客户分辨率大小(此分辨率比桌面分辨率小,因为去掉了任务栏的大小)的界面（宽desktopRect.width()，高esktopRect.height().
		//	this->parentWidget()->setGeometry(m_CurrentFactRect);
		//	m_CorrectionType = FALSE;
		//}
		//else
		//{
		//	//从当前进程屏幕上左上角位置开始，显示一个当前桌面客户分辨率大小(此分辨率比桌面分辨率小,因为去掉了任务栏的大小)的界面（宽desktopRect.width()，高esktopRect.height().
		//	this->parentWidget()->setGeometry(m_CurrentFactRect);
		//	m_CorrectionType = FALSE;
		//	m_PreviousFactRect = m_CurrentFactRect;
		//}

		this->parentWidget()->setGeometry(m_CurrentFactRect);
		m_PreviousFactRect = m_CurrentFactRect;
	//}
}










//功能函数区

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

// 加载本地样式文件(没做完);
// 可以将样式直接写在文件中，程序运行时直接加载进来;
void MyTitleBar::loadStyleSheet(const QString& sheetName)
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

//获取当前桌面客户区起始坐标及大小
QRect MyTitleBar::getDesktopRect()
{
	//此处为了兼容多屏幕,判断当前进程所在屏幕的序号
	QDesktopWidget* deskTop = QApplication::desktop();
	int curMonitor = deskTop->screenNumber(this->parentWidget()); // 参数是一个QWidget*
	//取所在屏幕桌面的客户大小(去掉任务栏后的大小)
	QRect desktopRect = QApplication::desktop()->availableGeometry(curMonitor);

	return desktopRect;
}

//监控线程调用的消息发送函数
void MyTitleBar::sendMonitormessage()
{
	emit monitorMessage_1();
}


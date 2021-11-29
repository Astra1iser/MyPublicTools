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
	, m_nPos(30) //30����25+5���ɵ� // ͼ������25,+5����Ϊ����ʱ������5����߾�
	, m_isChange(0)
	, m_nClickTimes(0)
	, m_isRightClickMenuOn(0)
	, m_TitleBarHeight(TITLE_HEIGHT)
	, m_CorrectionType(0)
	, m_TitleMonitor(new TitleMonitor(this))
	, m_isStretch(TRUE)
	, m_isMaximize(FALSE)
{
	// ��ʼ��;
	initControl();
	initConnections();
	// ���ر�����ʽ MyTitle.css�ļ�;
	//loadStyleSheet(QString::fromLocal8Bit(""));
	// ����߳�����
	m_TitleMonitor->Start();
}

MyTitleBar::~MyTitleBar()
{
	//����ָ��ָ�������Ϊ�ض������
	delete m_frame;
	delete m_TitleMonitor;
	delete m_pIcon;
	delete m_pTitleContent;
	delete m_pButtonMin;
	delete m_pButtonRestore;
	delete m_pButtonMax;
	delete m_pButtonClose;
	delete mylayout;

	//����ָ����������˿ռ�
	if (m_isRightClickMenuOn)
	{
		delete m_RightButtonMenu;
		delete m_MenuRestore;
		delete m_MenuMin;
		delete m_MenuMax;
		delete m_MenuClose;
	}
}

// ��ʼ���ؼ�;
void MyTitleBar::initControl()
{
	m_frame = new MyFrame(this->parentWidget());	//�߿�

	m_pIcon = new QLabel;
	m_pTitleContent = new QLabel;

	m_pButtonMin = new QPushButton;			//��С����ť
	m_pButtonRestore = new QPushButton;		//��ԭ��ť
	m_pButtonMax = new QPushButton;			//��󻯰�ť
	m_pButtonClose = new QPushButton;		//�رհ�ť


	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	m_pButtonMin->setToolTip(QStringLiteral("��С��"));
	m_pButtonRestore->setToolTip(QStringLiteral("��ԭ"));
	m_pButtonMax->setToolTip(QStringLiteral("���"));
	m_pButtonClose->setToolTip(QStringLiteral("�ر�"));

	mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	//�������QSS https://blog.csdn.net/Andy_93/article/details/53100640?utm_source=blogxgwz9
	//����ͼ�������Լ�����,����б�����Ҫ��,����ʱû���ṩ�ⲿ����,�����������,��ʵҲ���԰���������ť����public��,Ȼ���ⲿ�޸�

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
	// �ñ�����͸��,��Ҫ�ڸ��·��ı�����ɫ
	m_pTitleContent->setAttribute(Qt::WA_TranslucentBackground, true);

	//���ø�����ͼ����,�������,���������������,һ��Ҫ������������¸���������������
	this->parentWidget()->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	/*
	//�˴��������ʵ�ֱ������ͱ߿�,��nativeEvent()���ٴ�ȥ��������
	//��Ҫ�Ĺؼ���WS_THICKFRAME
	*/
	HWND m_ParentHwnd = (HWND)this->parentWidget()->winId();
	DWORD style = ::GetWindowLong(m_ParentHwnd, GWL_STYLE);
	::SetWindowLong(m_ParentHwnd, GWL_STYLE, style | WS_THICKFRAME);
	//����һ�����صı߿��ȣ�����ϵͳ������Ʊ߿���Ӱ
	const MARGINS shadow = { 1, 1, 1, 1 };
	//����ƽ����Ӱwin10���²�֧��
	DwmExtendFrameIntoClientArea(m_ParentHwnd, &shadow);
	//����������Ӱȫϵͳ֧��
	SetClassLong(m_ParentHwnd, GCL_STYLE, GetClassLong(m_ParentHwnd, GCL_STYLE) | CS_DROPSHADOW);//������Ӱ

	//BOOL bEnable = false;
	//::DwmIsCompositionEnabled(&bEnable);

	//if (bEnable)
	//{
	//	DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
	//	::DwmSetWindowAttribute(m_ParentHwnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
	//	MARGINS margins = { -1 };
	//	//����ƽ����Ӱwin10���²�֧��
	//	::DwmExtendFrameIntoClientArea(m_ParentHwnd, &margins);
	//}

	//this->setWindowFlags(Qt::FramelessWindowHint);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	this->setTitleWidth();
	this->setTitleHeight();
}

//����(����)������߿�
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
		//�ϱ߽�
		m_frame->m_Top_Horizontal_Line->setGeometry(QRect(-1, 0, this->parentWidget()->width()+2, 2));
		//�±߽�
		m_frame->m_Bottom_Horizontal_Line->setGeometry(QRect(0, this->parentWidget()->height()-1, this->parentWidget()->width()+2, 2));
		//��߽�
		m_frame->m_Left_Vertical_Line->setGeometry(QRect(0, 0, 2, this->parentWidget()->height()));
		//�ұ߽�
		m_frame->m_Right_Vertical_Line->setGeometry(QRect(this->parentWidget()->width()-1, -1, 2, this->parentWidget()->height()+2));
		return;
	}
	else
	{	
		//�ϱ߽�
		m_frame->m_Top_Horizontal_Line->setGeometry(QRect(-1, 0, this->parentWidget()->width()+2, 0));
		//�±߽�
		m_frame->m_Bottom_Horizontal_Line->setGeometry(QRect(0, this->parentWidget()->height() - 1, this->parentWidget()->width()+2, 2));
		//��߽�
		m_frame->m_Left_Vertical_Line->setGeometry(QRect(0, m_TitleBarHeight, 2, this->parentWidget()->height()- m_TitleBarHeight));
		//�ұ߽�
		m_frame->m_Right_Vertical_Line->setGeometry(QRect(this->parentWidget()->width() - 1, -1, 2, this->parentWidget()->height() + 2));
		return;
	}
}

// �źŲ۵İ�;
void MyTitleBar::initConnections()
{
	//ע�������ʱ�����������жϵ�˫����,һ����Ҫɾ��
	connect(&m_cTimer, SIGNAL(timeout()), this, SLOT(slotTimerTimeOut()));

	//�����ǰ�ť���źŲ�
	connect(m_pButtonMin, SIGNAL(pressed()), this, SLOT(onButtonMinPressed()));
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonMax, SIGNAL(pressed()), this, SLOT(onButtonMaxPressed()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonRestore, SIGNAL(pressed()), this, SLOT(onButtonRestorePressed()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonClose, SIGNAL(pressed()), this, SLOT(onButtonClosePressed()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));

	//�����Ǽ�����߳��źŲ�,�����Զ�������
	connect(this, SIGNAL(monitorMessage_1()), this, SLOT(CorrectionWindow()));
}








//�û����ú�����

// ���ñ���������ɫ,��paintEvent�¼��н��л��Ʊ���������ɫ;
// �ڹ��캯���и���Ĭ��ֵ�������ⲿ������ɫֵ�ı����������ɫ;
void MyTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
	m_colorR = r;
	m_colorG = g;
	m_colorB = b;
	m_isTransparent = isTransparent;
	// ���»��ƣ�����paintEvent�¼���;
	update();
}

// ���ñ�����ͼ��;
void MyTitleBar::setTitleIcon(QString filePath, QSize IconSize)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setPixmap(titleIcon.scaled(IconSize));
	//m_nPos = m_pIcon->width();
}

// ���ñ�������;
void MyTitleBar::setTitleContent(QString titleContent, int titleFontSize, const char* ColorStyle)
{
	// ���ñ��������С;
	QFont font = m_pTitleContent->font();
	font.setBold(true);
	font.setPointSize(titleFontSize);
	m_pTitleContent->setFont(font);
	//m_pTitleContent->setStyleSheet("color:white;"); 
	m_pTitleContent->setStyleSheet(ColorStyle); 
	// ���ñ�������;
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

// ���ñ���������;(�������Ŀǰû��,Ĭ�ϱ��������ǳ���������,����ʹ����setWindowBorderWidth()�����˸������б߿��ߵĿ��)
void MyTitleBar::setTitleWidth(int width)
{
	if (0 == width)
	{
		width = this->parentWidget()->width();
	}
	this->setFixedWidth(width);

}

// ���ñ������߶�(����Ϊ30,����30��һ����Ϊ��30,��Ϊ̫���˻���β��ÿ�,��������Լ�������Ĵ���);
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

// ���ô��ڱ߿���;
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
	m_windowBorderWidth = borderWidth;
}

// ���ñ������ϰ�ť����;
// ���ڲ�ͬ���ڱ������ϵİ�ť����һ�������Կ����Զ���������еİ�ť;
// �����ṩ���ĸ���ť���ֱ�Ϊ��С������ԭ����󻯡��رհ�ť�������Ҫ������ť�������������;
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

// ���ñ������еı����Ƿ���Զ�����������Ƶ�Ч��;
// һ������±������еı��������ǲ�������
void MyTitleBar::setTitleRoll(int timeInterval)
{
	m_nPos = m_pIcon->width() + 5; //���ù�����ʼλ�� +5����Ϊ����ʱ������5����߾�
	connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
	m_titleRollTimer.start(timeInterval);
}

//�����Ҽ��˵�
void MyTitleBar:: setRightClickMenu()
{
	//�ο� https://blog.csdn.net/qq_33266987/article/details/54017072?utm_medium=distribute.pc_feed_404.none-task-blog-2~default~BlogCommendFromBaidu~default-8.test_5searchdownload&depth_1-utm_source=distribute.pc_feed_404.none-task-blog-2~default~BlogCommendFromBaidu~default-8.test_5searchdownloa
	
	if (m_isRightClickMenuOn)
	{
		return;
	}

	m_isRightClickMenuOn = TRUE;
	this->setContextMenuPolicy(Qt::CustomContextMenu);//����Ҽ��˵�����
	m_RightButtonMenu = new QMenu(this); //�˵�ʵ���ָ��

	//ѡ��
	m_MenuRestore = new QAction(QIcon("image/restoreBlack_16.png"), YString("��ԭ"), this); //��ԭ
	m_MenuMin = new QAction(QIcon("image/minimizeBtnBlack_16.png"), YString("��С��"), this); //��С��
	m_MenuMax = new QAction(QIcon("image/maximizeBtnBlack_16.png"), YString("���"), this); //���
	m_MenuClose = new QAction(QIcon("image/closeBtnBlack_16.png"), YString("�ر�"), this); //�ر�

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

// ���õ�ǰ�����Ƿ������
void MyTitleBar::setStretch(BOOL Options)
{
	m_isStretch = Options;
}







//�¼���

// ���Ʊ���������ɫ;
void MyTitleBar::paintEvent(QPaintEvent *event)
{
	// �Ƿ����ñ���͸��;
	if (!m_isTransparent)
	{
		//���ñ���ɫ;
		QPainter painter(this);
		QPainterPath pathBack;
		pathBack.setFillRule(Qt::WindingFill);
		//�����Ϸ������Ϸ�����Բ�� ����ֱ������0,����Ҫ���Լ���
		pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 0, 0);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
	}   

	// ��������󻯻��߻�ԭ�󣬴��ڳ��ȱ��ˣ��������ĳ���Ӧ��һ��ı�;
	// �����ȥm_windowBorderWidth ������Ϊ���ڿ��������˲�ͬ��ȵı߿�;
	// ��������б߿�����Ҫ����m_windowBorderWidth��ֵ������m_windowBorderWidthĬ��Ϊ0;
	if (this->width() != (this->parentWidget()->width() - 2*m_windowBorderWidth))
	{
		//���ñ�������С�ɸ���,��Ȼ��QT�Ĵ������Ի����˹�����(�ñ�������ʧ)
		if (this->parentWidget()->width() - 2 * m_windowBorderWidth > 0)
		{
			this->setFixedWidth(this->parentWidget()->width() - 2 * m_windowBorderWidth);
		}
	}

	//�жϸ߶��Ƿ�����,������ʵ���������ж�,����Ϊ�˶�Ӧ����ж�,���Ǽ��ϰ�
	if (this->height() < TITLE_HEIGHT)
	{
		this->setFixedHeight(TITLE_HEIGHT);
	}
	//�����˱߿�ʱ���Һ��Ϸ�Ҫ�ڳ���Ӧ��С�ı߿�λ��
	this->move(m_windowBorderWidth, m_windowBorderWidth);
	setFrame();
	QWidget::paintEvent(event);
}

// ˫����Ӧ�¼�����Ҫ��ʵ��˫��������������󻯺���С������;
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	// ֻ�д�����󻯡���ԭ��ťʱ˫������Ч;
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		// ͨ����󻯰�ť��״̬�жϵ�ǰ�����Ǵ�����󻯻���ԭʼ��С״̬;
		// ����ͨ���������ñ�������ʾ��ǰ����״̬;
		if (m_pButtonMax->isVisible() && !this->m_pIcon->underMouse())
		{
			if (event->button() == Qt::LeftButton && DOUBLECLICK == m_nClickTimes+1)//�����1����Ϊ˫���¼������ڵ���ǰ,��ʱm_nClickTimes��û����1
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

// ����ͨ��mousePressEvent��mouseMoveEvent��mouseReleaseEvent�����¼�ʵ��������϶��������ƶ����ڵ�Ч��;
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
	//���������,ÿ��һ�¼�һ,�����жϵ���˫����
	m_nClickTimes++;
	//��˫����ʱ��,�����뿴slotTimerTimeOut()
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
	//�������ƶ�ʱ,���ֵ�ǰ����Ļָ���ť����,�ҿ����϶��򴥷���ԭ�߼�(��΢�����)
	if (m_pButtonRestore->isVisible() && isTitleUnderMouse  /* && m_CanMove == 1*/)
	{
		//������ǻ�ԭ΢����Ķ���,����ƶ�����ʱ,��ԭԭʼ���岢��������ƶ�
		//����һ�δ��廹ԭ
		onButtonRestoreClicked();
		//m_CanMove = 0;
		QPoint movePoint = event->globalPos();
		//�ø����ڵı����м��׼���
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








//�ۺ�����

//��˫����ʱ���ۺ���
void MyTitleBar::slotTimerTimeOut()
{
	/*
	* https://blog.csdn.net/beibeix2015/article/details/77488910
	������ if (event->button() == Qt::LeftButton && m_nClickTimes == 1)
	���ж�����������,���˫��,������,�Ҽ�����,�Ҽ�˫��,�Ҽ����,������ô�ü�������
	��ʱ��ֹͣ, �����ʱ��ʱ���ڵĵ������, ���������Ϊ1ʱ��Ϊ��λʱ���ڵ����һ��, �������Ϊ2ʱ��Ϊ��λʱ���ڵ��������
	ע���ʱ����ʱ�䲻Ҫ����,��Ȼ���ڴ�����˵,̫�̻���ɼ�������Ϊ1,��mousePressEvent()���жϵ���ʱ��һֱ�ɹ�,����˫��ʱ��������
	��ʱ����ʱ��Ҳ����̫��,��Ȼ���ڴ�����˵,��������Ϊ�жϵ����1ʱ,�����һ���жϳɹ��ĵ����ӳٻᱻ�ӳ�
	�����Ƽ���ʱ��ʱ������Ϊ200-400�����
	*/
	m_cTimer.stop();
	//if (1 == m_nClickTimes)
	//{
	//	//���� TODU
	//	qDebug() < <YString("����") << endl;
	//}
	//else if (2 == m_nClickTimes)
	//{
	//	//˫�� TODU
	//	qDebug() << YString("˫��") << endl;
	//}

	//������������
	m_nClickTimes = 0;

}

//�Ҽ��˵��Ĳۺ���
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
				m_RightButtonMenu->addSeparator();    //�ָ���
				m_RightButtonMenu->addAction(m_MenuClose);
			}

			if (MIN_MAX_BUTTON == m_buttonType)
			{
				m_RightButtonMenu->clear();
				m_RightButtonMenu->addAction(m_MenuRestore);
				m_RightButtonMenu->addAction(m_MenuMin);
				m_RightButtonMenu->addAction(m_MenuMax);
				m_RightButtonMenu->addSeparator();    //�ָ���
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
				m_RightButtonMenu->exec(QCursor::pos());  //�ڵ�ǰ��괦��ס
			else
			{
				if (m_CursorIsLeftClick)
					m_RightButtonMenu->exec(pos);  //���ض����괦��ס
				else
					m_RightButtonMenu->exec(QCursor::pos());  //�ڵ�ǰ��괦��ס
			}

			isTitleUnderMouse = this->m_pIcon->underMouse()
				&& !this->m_pButtonMin->underMouse()
				&& !this->m_pButtonMax->underMouse()
				&& !this->m_pButtonRestore->underMouse()
				&& !this->m_pButtonClose->underMouse();
		}
	}
}

// ����Ϊ��ť������Ӧ�Ĳ�;
//��С����Ӧ����,���ʱ
void MyTitleBar::onButtonMinPressed()
{
	isTitleUnderMouse = FALSE;
}

//��С����Ӧ����,�����
void MyTitleBar::onButtonMinClicked()
{
	this->parentWidget()->showMinimized();
}

//�����Ӧ����,���ʱ
void MyTitleBar::onButtonMaxPressed()
{
	isTitleUnderMouse = FALSE;
}

//�����Ӧ����,�����
void MyTitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);

	//����һ���û����õ���������,windows���߼�����󻯽�ֹ����,���������ʱҲ��ʱ���ò�������
	m_isStretch_buffer = m_isStretch;
	m_isStretch = FALSE;

	//����һ��ԭʼ�����С,���ڻ�ԭʱ��
	this->saveRestoreInfo(this->parentWidget()->pos(), QSize(this->parentWidget()->width(), this->parentWidget()->height()));

	QRect desktopRect = getDesktopRect();
	//�ӵ�ǰ������Ļ�����Ͻ�λ�ÿ�ʼ����ʾһ����ǰ����ͻ��ֱ��ʴ�С(�˷ֱ��ʱ�����ֱ���С,��Ϊȥ�����������Ĵ�С)�Ľ��棨��desktopRect.width()����esktopRect.height().
	this->parentWidget()->setGeometry(desktopRect);

	m_isMaximize = TRUE;
	m_pButtonRestore->setVisible(true);
}

//��ԭ��Ӧ����,���ʱ
void MyTitleBar::onButtonRestorePressed()
{
	isTitleUnderMouse = FALSE;
}

//��ԭ��Ӧ����,�����
void MyTitleBar::onButtonRestoreClicked()
{
	m_pButtonRestore->setVisible(false);

	//������ѡ�ԭ
	m_isStretch = m_isStretch_buffer;

	QPoint windowPos;
	QSize windowSize;
	//��֮ǰ�����ԭʼ�����Сȡ������ԭ
	this->getRestoreInfo(windowPos, windowSize);
	this->parentWidget()->setGeometry(QRect(windowPos, windowSize));
	//this->parentWidget()->setWindowState(Qt::WindowActive);

	m_isMaximize = FALSE;
	m_pButtonMax->setVisible(true);
}

//�ر���Ӧ����,���ʱ
void MyTitleBar::onButtonClosePressed()
{
	isTitleUnderMouse = FALSE;
}

//�ر���Ӧ����,�����
void MyTitleBar::onButtonCloseClicked()
{
	this->parentWidget()->close();
}

// ��������ۺ���,�÷�����Ҫ���ñ������еı�����ʾΪ������Ч��;
void MyTitleBar::onRollTitle()
{
	QFontMetrics fm(m_pTitleContent->font());
	int textWidthInPxs = fm.width(m_titleContent);

	if ((m_nPos + textWidthInPxs) >= (this->width() - m_ButtonWidthCount))
	{
		m_nPos = this->width() - m_ButtonWidthCount - textWidthInPxs;
		m_isChange = TRUE;
	}

	if (m_nPos <= m_pIcon->width() + 5)	// +5����Ϊ����ʱ������5����߾�
	{
		m_nPos = m_pIcon->width() + 5;	// +5����Ϊ����ʱ������5����߾�
		m_isChange = FALSE;
	}

	m_pTitleContent->move(m_nPos, (m_pTitleContent->pos().y()));

	if (FALSE == m_isChange)
		m_nPos++;
	else
		m_nPos--;
}

//��󻯺�,��������������ķֱ��ʻ��߿ͻ������ַ����ı�����������ۺ���
void MyTitleBar::CorrectionWindow()
{
	//�������ñ�־��ԭ���ǽ����������Ż�,��Ȼһֱˢ��������Ϣ�������Ĺ���
	//if (m_CorrectionType == TRUE && m_isMaximize)
	//if (m_isMaximize)
	//{
		//m_CurrentFactRect = getDesktopRect();

		//if (m_CurrentFactRect == m_PreviousFactRect)
		//{
		//	//�ӵ�ǰ������Ļ�����Ͻ�λ�ÿ�ʼ����ʾһ����ǰ����ͻ��ֱ��ʴ�С(�˷ֱ��ʱ�����ֱ���С,��Ϊȥ�����������Ĵ�С)�Ľ��棨��desktopRect.width()����esktopRect.height().
		//	this->parentWidget()->setGeometry(m_CurrentFactRect);
		//	m_CorrectionType = FALSE;
		//}
		//else
		//{
		//	//�ӵ�ǰ������Ļ�����Ͻ�λ�ÿ�ʼ����ʾһ����ǰ����ͻ��ֱ��ʴ�С(�˷ֱ��ʱ�����ֱ���С,��Ϊȥ�����������Ĵ�С)�Ľ��棨��desktopRect.width()����esktopRect.height().
		//	this->parentWidget()->setGeometry(m_CurrentFactRect);
		//	m_CorrectionType = FALSE;
		//	m_PreviousFactRect = m_CurrentFactRect;
		//}

		this->parentWidget()->setGeometry(m_CurrentFactRect);
		m_PreviousFactRect = m_CurrentFactRect;
	//}
}










//���ܺ�����

// ���洰�����ǰ���ڵ�λ���Լ���С;
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

// ��ȡ�������ǰ���ڵ�λ���Լ���С;
void MyTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}

// ���ر�����ʽ�ļ�(û����);
// ���Խ���ʽֱ��д���ļ��У���������ʱֱ�Ӽ��ؽ���;
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

//��ȡ��ǰ����ͻ�����ʼ���꼰��С
QRect MyTitleBar::getDesktopRect()
{
	//�˴�Ϊ�˼��ݶ���Ļ,�жϵ�ǰ����������Ļ�����
	QDesktopWidget* deskTop = QApplication::desktop();
	int curMonitor = deskTop->screenNumber(this->parentWidget()); // ������һ��QWidget*
	//ȡ������Ļ����Ŀͻ���С(ȥ����������Ĵ�С)
	QRect desktopRect = QApplication::desktop()->availableGeometry(curMonitor);

	return desktopRect;
}

//����̵߳��õ���Ϣ���ͺ���
void MyTitleBar::sendMonitormessage()
{
	emit monitorMessage_1();
}


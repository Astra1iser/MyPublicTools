#include "MyTitleBar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>

#define BUTTON_HEIGHT 30        // ��ť�߶�;
#define BUTTON_WIDTH 30         // ��ť���;
#define TITLE_HEIGHT 30         // �������߶�;

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
	// ��ʼ��;
	initControl();
	initConnections();
	// ���ر�����ʽ MyTitle.css�ļ�;
	//loadStyleSheet(QString::fromLocal8Bit("������"));
}

MyTitleBar::~MyTitleBar()
{

}

// ��ʼ���ؼ�;
void MyTitleBar::initControl()
{
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


	//���ø�����ͼ����,�������
	this->parentWidget()->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	//this->setWindowFlags(Qt::FramelessWindowHint);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	this->setTitleWidth();
	this->setTitleHeight();
}

// �źŲ۵İ�;
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
	m_nPos = m_pIcon->width();
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
		height = TITLE_HEIGHT;
	}
	this->setFixedHeight(height);
	//m_height = this->height();
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

// ���ñ������еı����Ƿ���Զ�����������Ƶ�Ч��;
// һ������±������еı��������ǲ�������
void MyTitleBar::setTitleRoll(int timeInterval)
{
	connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
	m_titleRollTimer.start(timeInterval);
}

// ���ô��ڱ߿���;
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
	m_windowBorderWidth = borderWidth;
}

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
		this->setFixedWidth(this->parentWidget()->width() - 2*m_windowBorderWidth);
	}
	//if (m_height - m_windowBorderWidth < 30)
	//�жϸ߶��Ƿ�����,������ʵ���������ж�,����Ϊ�˶�Ӧ����ж�,���Ǽ��ϰ�
	if (this->height() < TITLE_HEIGHT)
	{
		this->setFixedHeight(TITLE_HEIGHT);
	}
	
	//�����˱߿�ʱ���Һ��Ϸ�Ҫ�ڳ���Ӧ��С�ı߿�λ��
	this->move(m_windowBorderWidth, m_windowBorderWidth);
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

// ����ͨ��mousePressEvent��mouseMoveEvent��mouseReleaseEvent�����¼�ʵ��������϶��������ƶ����ڵ�Ч��;
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
	//�����ǰ�ָ���ť����,֤���������󻯻���˫����������
	//if (m_pButtonRestore->isVisible() && !m_pButtonMax->isVisible() && this->geometry().contains(this->mapFromGlobal(QCursor::pos()))/*&& this->parentWidget()->windowState() == Qt::WindowMaximized*/ )
	//{
	//	//�õ�ǰ�Ĵ�������յ��ƶ������������
	//	m_CanMove = 1;
	//}
	//else
	//{
	//	//���õ�ǰ�Ĵ�������յ��ƶ������������
	//	m_CanMove = 0;
	//}

	//�������ƶ�ʱ,���ֵ�ǰ����Ļָ���ť����,�ҿ����϶��򴥷���ԭ�߼�(��΢�����)
	if (m_pButtonRestore->isVisible() && this->geometry().contains(this->mapFromGlobal(QCursor::pos())) /* && m_CanMove == 1*/)
	{
		//������ǻ�ԭ΢����Ķ���,����ƶ�����ʱ,��ԭԭʼ���岢��������ƶ�
		//����һ�δ��廹ԭ
		onButtonRestoreClicked();
		//m_CanMove = 0;
		QPoint movePoint = event->globalPos();
		//�ø����ڵı����м��׼���
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

// ���ر�����ʽ�ļ�;
// ���Խ���ʽֱ��д���ļ��У���������ʱֱ�Ӽ��ؽ���;
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

// ����Ϊ��ť������Ӧ�Ĳ�;
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

// �÷�����Ҫ���ñ������еı�����ʾΪ������Ч��;
void MyTitleBar::onRollTitle()
{
	//static int nPos = 0;
	//QString titleContent = m_titleContent;
	//// ����ȡ��λ�ñ��ַ�����ʱ����ͷ��ʼ;
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



// ����Ϊ��ť������Ӧ�Ĳ�;
void MyTitleBar::onButtonMinClicked2()
{
	this->parentWidget()->showMinimized();
}

void MyTitleBar::onButtonRestoreClicked2()
{
	this->parentWidget()->setWindowState(Qt::WindowNoState);
	//ԭʼ���ڿ�����չ��С(��windows���)
	this->parentWidget()->setMaximumHeight(16777215);
	this->parentWidget()->setMaximumWidth(16777215);
	this->parentWidget()->setMinimumHeight(0);
	this->parentWidget()->setMinimumWidth(0);

	QPoint windowPos;
	QSize windowSize;
	//��֮ǰ�����ԭʼ�����Сȡ������ԭ
	this->getRestoreInfo(windowPos, windowSize);
	this->parentWidget()->setGeometry(QRect(windowPos, windowSize));
}

void MyTitleBar::onButtonMaxClicked2()
{
	//����һ��ԭʼ�����С,���ڻ�ԭʱ��
	this->saveRestoreInfo(this->parentWidget()->pos(), QSize(this->parentWidget()->width(), this->parentWidget()->height()));

	//�˴�Ϊ�˼��ݶ���Ļ,�жϵ�ǰ����������Ļ�����
	QDesktopWidget* deskTop = QApplication::desktop();
	int curMonitor = deskTop->screenNumber(this); // ������һ��QWidget*
	//ȡ������Ļ����Ŀͻ���С(ȥ����������Ĵ�С)
	QRect desktopRect = QApplication::desktop()->availableGeometry(curMonitor);
	QRect FactRect = QRect(desktopRect.x(), desktopRect.y(), desktopRect.width(), desktopRect.height());

	//�ӵ�ǰ������Ļ�����Ͻ�λ�ÿ�ʼ����ʾһ����ǰ����ͻ��ֱ��ʴ�С(�˷ֱ��ʱ�����ֱ���С,��Ϊȥ�����������Ĵ�С)�Ľ��棨��desktopRect.width()����esktopRect.height().
	this->parentWidget()->setGeometry(FactRect);
	//this->parentWidget()->setWindowState(Qt::WindowMaximized);

	//���ʱ��ֹ���촰��(��windows���)
	this->parentWidget()->setMinimumHeight(this->parentWidget()->height());
	this->parentWidget()->setMinimumWidth(this->parentWidget()->width());
	this->parentWidget()->setMaximumHeight(this->parentWidget()->height());
	this->parentWidget()->setMaximumWidth(this->parentWidget()->width());
}

void MyTitleBar::onButtonCloseClicked2()
{
	this->parentWidget()->close();
}

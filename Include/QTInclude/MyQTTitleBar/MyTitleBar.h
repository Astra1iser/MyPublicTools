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
	MIN_BUTTON = 0,         // ��С���͹رհ�ť;
	MIN_MAX_BUTTON ,        // ��С������󻯺͹رհ�ť;
	ONLY_CLOSE_BUTTON       // ֻ�йرհ�ť;
};














//���ʹ������Զ��������?

//������Ҫ�ڴ����г�ʼ���Զ��������������дĳЩ����
/* 
protected:
	MyTitleBar* m_titleBar;
	
private:
	void initTitleBar();									//��ʼ������
	
private slots:
	void onButtonMinClicked();								//��С���ۺ���
	void onButtonRestoreClicked();							//��ԭ���ۺ���
	void onButtonMaxClicked();								//��󻯲ۺ���
	void onButtonCloseClicked();							//�رհ�ť�ۺ��� */

//Ȼ������ʵ�����º���
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




//Ȼ�����������г�ʼ���������(һ��Ҫ�� ui.setupUi(this); ��)
/* void QAXSafeCheckerTrustDlg ::initTitleBar()
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	
	// ���ô��ڱ���͸��;
	this->setAttribute(Qt::WA_TranslucentBackground);


	m_titleBar = new MyTitleBar(this);
	m_titleBar->move(0, 0);
	m_titleBar->setTitleIcon("../Image/QAXico.png");
	m_titleBar->setTitleContent(QStringLiteral(" ���Ǳ���"),10);
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
	//����parentû�и�Ĭ��ֵNULL����֤�ڴ���MyTitleBar����ʱ��ָ�����ø�ֵ;�Ҹ�ֵ��ΪNULL;
	~MyTitleBar();

	// ���ñ���������ɫ���Ƿ����ñ���������ɫ͸��;
	void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
	// ���ñ�����ͼ��;
	void setTitleIcon(QString filePath , QSize IconSize = QSize(25 , 25));
	// ���ñ�������;
	void setTitleContent(QString titleContent , int titleFontSize = 9);
	// ���ñ���������;
	void setTitleWidth(int width);
	// ���ñ������ϰ�ť����;
	void setButtonType(ButtonType buttonType);
	// ���ñ������еı����Ƿ�����;������Կ�Ч��;
	void setTitleRoll();
	// ���ô��ڱ߿���;
	void setWindowBorderWidth(int borderWidth);

	// ����/��ȡ ���ǰ���ڵ�λ�ü���С;
	void saveRestoreInfo(const QPoint point, const QSize size);
	void getRestoreInfo(QPoint& point, QSize& size);

private:
	void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	// ��ʼ���ؼ�;
	void initControl();
	// �źŲ۵İ�;
	void initConnections();
	// ������ʽ�ļ�;
	void loadStyleSheet(const QString &sheetName);

signals:
	// ��ť�������ź�;
	void signalButtonMinClicked();
	void signalButtonRestoreClicked();
	void signalButtonMaxClicked();
	void signalButtonCloseClicked();

	private slots:
		// ��ť�����Ĳ�;
		void onButtonMinClicked();
		void onButtonRestoreClicked();
		void onButtonMaxClicked();
		void onButtonCloseClicked();
		void onRollTitle();

private:
	QLabel* m_pIcon;                    // ������ͼ��;
	QLabel* m_pTitleContent;            // ����������;
	QPushButton* m_pButtonMin;          // ��С����ť;
	QPushButton* m_pButtonRestore;      // ��󻯻�ԭ��ť;
	QPushButton* m_pButtonMax;          // ��󻯰�ť;
	QPushButton* m_pButtonClose;        // �رհ�ť;

	// ����������ɫ;
	int m_colorR;
	int m_colorG;
	int m_colorB;

	// ��󻯣���С������;
	QPoint m_restorePos;
	QSize m_restoreSize;
	// �ƶ����ڵı���;
	bool m_isPressed;
	QPoint m_startMovePos;
	// �����������Ч��ʱ��;
	QTimer m_titleRollTimer;
	// ����������;
	QString m_titleContent;
	// ��ť����;
	ButtonType m_buttonType;
	// ���ڱ߿���;
	int m_windowBorderWidth;
	// �������Ƿ�͸��;
	bool m_isTransparent;
};

#endif
#pragma   once
#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H
#include <QtWidgets>
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

//���������� ����ë����Ч��(���ò����Ҫ��װһ��������,��Ҫ�ڱ�����������)
//��������
//ͼ�����Ҽ��˵�


//ps:�Ҿ����ҽ�����Ҫ�ο������https://blog.csdn.net/a844651990/article/details/106985554/


//���ʹ������Զ��������?

//������Ҫ�ڴ����г�ʼ���Զ������������
/* 
protected:
	MyTitleBar* m_titleBar;
	
private:
	void initTitleBar();									//��ʼ���Զ��������
	
*/




//Ȼ�����������г�ʼ���������(һ��Ҫ�� ui.setupUi(this); ��)
/* void QAXSafeCheckerTrustDlg ::initTitleBar()
{
	m_titleBar = new MyTitleBar(this);
	m_titleBar->setTitleIcon("../Image/QAXico.png");
	m_titleBar->setTitleContent(QStringLiteral(" ���Ǳ���"),10);
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());



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
	void setTitleContent(QString titleContent , int titleFontSize = 9, const char* ColorStyle = "color:white;");
	// ���ñ���������;
	void setTitleWidth(int width = 0);
	// ���ñ������߶�;
	void setTitleHeight(int height = 0);
	// ���ñ������ϰ�ť����;
	void setButtonType(ButtonType buttonType);
	// ���ñ������еı����Ƿ�����(ֵ��Ҫ̫С,������Cpu����);
	void setTitleRoll(int timeInterval = 5);
	// ���ô��ڱ߿���(������������б߿�ĳ���,���ú�,�����������Һ��Ϸ���ճ���Ӧ��С�Ŀ��������ʾ�����ڱ߿�);
	void setWindowBorderWidth(int borderWidth);


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

//signals:
//	// ��ť�������ź�;
//	void signalButtonMinClicked();
//	void signalButtonRestoreClicked();
//	void signalButtonMaxClicked();
//	void signalButtonCloseClicked();

private slots:
	// ��ť�����Ĳ�;
	void onButtonMinClicked();
	void onButtonMinPressed();
	void onButtonMaxClicked();
	void onButtonMaxPressed();
	void onButtonRestoreClicked();
	void onButtonRestorePressed();
	void onButtonCloseClicked();
	void onButtonClosePressed();
	void onRollTitle();



	////ÿ����ť�ľ���ʵ��(�ѷ���)
	//void onButtonMinClicked2();
	//void onButtonRestoreClicked2();
	//void onButtonMaxClicked2();
	//void onButtonCloseClicked2();


private:
	QLabel* m_pIcon;                    // ������ͼ��;
	QLabel* m_pTitleContent;            // ����������;
	QLabel* m_pTitleContent2;            // ����������;
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
	// ����������;
	QString m_titleContent2;
	// ��ť����;
	ButtonType m_buttonType;
	// ���ڱ߿���;
	int m_windowBorderWidth;
	// �������Ƿ�͸��;
	bool m_isTransparent;

	int m_nPos; //��������
	BOOL m_isChange; //�����Ƿ�ת������
	//BOOL m_CanMove; //�����Ƿ������ʱ���ƶ�
	//BOOL m_CanMove2; //�����Ƿ������ʱ���ƶ�
	//int m_height;	//�����˵�ǰ�����FixedHeight,��ֵ����Ϊ30,��ֵ��ʵ������ڸ������Y������,�����Ǵ���߶�

	// ����/��ȡ ���ǰ���ڵ�λ�ü���С;
	void saveRestoreInfo(const QPoint point, const QSize size);
	void getRestoreInfo(QPoint& point, QSize& size);
	BOOL isTitleUnderMouse;
};

#endif
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
	MIN_BUTTON = 0,         // ��С���͹رհ�ť;
	MIN_MAX_BUTTON ,        // ��С������󻯺͹رհ�ť;
	ONLY_CLOSE_BUTTON       // ֻ�йرհ�ť;
};

enum MouseClickTimes
{
	NOCLICK = 0,
	SINGLECLICK,
	DOUBLECLICK,
	OTHERTIMESCLICK
};

#define BUTTON_HEIGHT 30        // ��ť�߶�;
#define BUTTON_WIDTH 42         // ��ť���;
#define TITLE_HEIGHT 30         // �������߶�;

//���������� ����ë����Ч��(���ò����Ҫ��װһ��������,��Ҫ�ڱ�����������)
//��������
//б������

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
	//��������Ϊ��Ҫ
	m_titleBar = new MyTitleBar(this);
	m_titleBar->setTitleIcon("../Image/QAXico.png");				//����ͼ��
	m_titleBar->setTitleContent(QStringLiteral(" ���Ǳ���"),10);	//���ñ���ͱ����С
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);					//������Ҫ�ļ������ܰ�ť

	//�������ÿ�ѡ
	// ���ñ���������ɫ���Ƿ����ñ���������ɫ͸��;
	void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
	// ���ñ������߶�;
	void setTitleHeight(int height = 0);
	// ���ñ������еı����Ƿ�����(ֵ��Ҫ̫С,���������Cpu����);
	void setTitleRoll(int timeInterval = 5);
	// �������ڿ��(���ú�,�����������Һ��Ϸ���ճ���Ӧ��С�Ŀ��);
	void setWindowBorderWidth(int borderWidth);
	// ���ÿ����Ҽ��˵�(����ֻ�����˻�������,����Ҫʲô�����Լ�ȥ��)
	void setRightClickMenu();
	// ���ô����Ƿ������
	void setStretch(BOOL Options = TRUE);

} */


class MyTitleBar : public QDialog
{
	Q_OBJECT

public:
	MyTitleBar(QWidget *parent);
	// ����parentû�и�Ĭ��ֵNULL����֤�ڴ���MyTitleBar����ʱ��ָ�����ø�ֵ;�Ҹ�ֵ��ΪNULL;
	~MyTitleBar();


//����ѡ��Ϊ�û�����
	// ���ñ���������ɫ���Ƿ����ñ���������ɫ͸��;
	void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
	// ���ñ�����ͼ��;
	void setTitleIcon(QString filePath , QSize IconSize = QSize(25 , 25));
	// ���ñ�������;
	void setTitleContent(QString titleContent , int titleFontSize = 9, const char* ColorStyle = "color:white;");
	// ���ñ���������(��ʱû��);
	void setTitleWidth(int width = 0);
	// ���ñ������߶�;
	void setTitleHeight(int height = 0);
	// ���ñ������ϰ�ť����;
	void setButtonType(ButtonType buttonType);
	// ���ñ������еı����Ƿ�����(ֵ��Ҫ̫С,���������Cpu����);
	void setTitleRoll(int timeInterval = 5);
	// �������ڿ��(���ú�,�����������Һ��Ϸ���ճ���Ӧ��С�Ŀ��);
	void setWindowBorderWidth(int borderWidth);
	// ���ÿ����Ҽ��˵�(����ֻ�����˻�������,����Ҫʲô�����Լ�ȥ��)
	void setRightClickMenu();
	// ���ô����Ƿ������
	void setStretch(BOOL Options = TRUE);

//�ⲿ�õ��ĺ���
public:
	//��ȡ�����û�����Ϣ(�������ʹ�С)
	QRect getDesktopRect();
	//����̷߳�����Ϣ�ĺ���
	void sendMonitormessage();

//�ⲿ��Ҫ������
public:
	BOOL m_CorrectionType;				// ����ֱ��ʸı��ʶ��,windows�¼����͵��˴���ʱ�Զ��ı�Ϊ1,���������ݺ���0
	QRect m_PreviousFactRect;			// �����滧���仯ǰ������ͻ�������
	BOOL m_isStretch;					// �û����õĴ����Ƿ����������
	BOOL m_isMaximize;					// ��ǰ�Ƿ������

//�ڲ�ʵ�ֺ���
private:
	// ��ʼ���ؼ�;
	void initControl();
	// �źŲ۵İ�;
	void initConnections();
	// ������ʽ�ļ�;
	void loadStyleSheet(const QString &sheetName);
	// ����/��ȡ ���ǰ���ڵ�λ�ü���С;
	void saveRestoreInfo(const QPoint point, const QSize size);
	void getRestoreInfo(QPoint& point, QSize& size);

//�ڲ��õ�������
private:
	QHBoxLayout* mylayout;
	QLabel* m_pIcon;                    // ������ͼ��;
	QLabel* m_pTitleContent;            // ����������;
	QPushButton* m_pButtonMin;          // ��С����ť;
	QPushButton* m_pButtonRestore;      // ��󻯻�ԭ��ť;
	QPushButton* m_pButtonMax;          // ��󻯰�ť;
	QPushButton* m_pButtonClose;        // �رհ�ť;
	int m_ButtonWidthCount;				// ��ť��ǰ���ܿ��

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

	QTimer m_titleRollTimer;			// �����������Ч��ʱ��;
	QString m_titleContent;				// ����������;
	int m_TitleBarHeight;				// ����߶�;
	bool m_isTransparent;				// �������Ƿ�͸��;
	int m_nPos;							// ���⿪ʼ����(��ʵ����ͼ����Ҳ�)
	BOOL m_isChange;					// �����Ƿ���Ҫת������

	QTimer m_cTimer;					// ��˫����ʱ��
	int m_nClickTimes;					// ���������
	BOOL m_CursorIsLeftClick;			// ������������

	QMenu* m_RightButtonMenu;			//	�Ҽ��˵�
	QAction* m_MenuRestore;				//	�Ҽ��˵���ԭѡ��
	QAction* m_MenuMin;					//	�Ҽ��˵���С��ѡ��
	QAction* m_MenuMax;					//	�Ҽ��˵����ѡ��
	QAction* m_MenuClose;				//	�Ҽ��˵��ر�ѡ��
	BOOL m_isRightClickMenuOn;			//	�û��Ƿ������Ҽ��˵�

	ButtonType m_buttonType;			// ��ť����;
	int m_windowBorderWidth;			// ���ڱ߿���;
	HWND m_ParentHwnd;					// ��������
	BOOL isTitleUnderMouse;				// ��ǰ����Ƿ��ڱ�������
	QRect m_CurrentFactRect;			// ��������滧���仯���(��ǰ��)����ͻ�������
	TitleMonitor* m_TitleMonitor;		// ������߳�ָ��
	BOOL m_isStretch_buffer;			// �û����õĴ����Ƿ���������Ե��ڲ�����,�����ڲ��л����Ժͻ�ԭ


//��д�����¼�����
private:
	void paintEvent(QPaintEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

//�ź�
signals:
	void monitorMessage_1();

//�ۺ���
private slots:
	// �жϵ�˫���ļ�ʱ���ۺ���
	void slotTimerTimeOut();
	// �Ҽ��˵��Ĳۺ���
	void slotPlayArgsMenu(const QPoint pos);
	// ��󻯺�,��������������ķֱ��ʻ��߿ͻ������ַ����ı��������������
	void CorrectionWindow();
	// ��������ۺ���
	void onRollTitle();
	// ��ť�����Ĳ�;
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


//���������߳�,������,����Դ���һ���߳��г�����ر������е��κ�����,��Ȼ�������һ��������
class TitleMonitor :public Thread
{
public:

	TitleMonitor(MyTitleBar* mtb)
	{
		this->mtb = mtb;
	}

	// ��дRun;
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
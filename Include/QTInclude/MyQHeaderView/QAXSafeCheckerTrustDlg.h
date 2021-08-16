#pragma   once
#ifndef QAXSAFECHECKERTRUSTDLG_H
#define QAXSAFECHECKERTRUSTDLG_H
#define ACCESS_READ								1  
#define ACCESS_WRITE							2  
#define IDS_TRUST_MD5							267
#define IDS_TRUST_PATH							268
#define IDS_TRUST_DIR							272
#define IDS_TRUST_MD5_AND_PATH		65103

#include <atlstr.h>
#include <iostream>
#include <comdef.h>
#include <sys/stat.h>

#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "Interface/quiplugin.h"
#include "Interface/ITrustRegion.h"
#include "Interface/CloudEngineSetting.h"

#include "MyQHeaderView.h"
#include "MyTitleBar.h"
//#include "BaseWindow.h"

#include "ui_QAXSafecheckerTrustDlg.h"
#include "AddFileDlg.h"
#include "AddDirDlg.h"
#include "AddExtDlg.h"
#include "TrustErrMsgDlg.h"
#include "DelWarningDlg.h"


#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QPushButton>

class QAXSafeCheckerTrustDlg : public QDialog
{
	Q_OBJECT

public:
	HMODULE  hDll;	//���ص�dll���
	MyQHeaderView* myHeader; //��д���ı�ͷ��
	ITrustRegion* pTrust;	//���ص�dll�е������ָ��
	CString CStrbuffer; //һ��������CString�������
	QString m_QStrFile;	//�������ļ�ʱ���ļ�·��
	QString m_QStrDir;	//������Ŀ¼ʱ��Ŀ¼·��
	QString m_QStrExt;	//���Ext��չ��
	int m_UserType;		//��ǰ��¼���û���Ȩ����(�û�/����Ա)
	ITrustZoneItem* m_pTrustItem;		//�ӿ����ļ���Ŀ¼����ָ��
	ITrustRegion* m_pTrustRegion;		//�ӿ���Ext����ָ��
	int ExtSelectRow;	//Ext�б��е�ǰѡ�����


	QAXSafeCheckerTrustDlg(QWidget *parent = 0);
	static QAXSafeCheckerTrustDlg* CreateInstance(QWidget* parent = Q_NULLPTR);	//�����Ӵ��ڶ���
	BOOL  QAXSafeCheckerTrustDlg::CreatePage();
	BOOL   IsAdmin();																			//�жϵ�ǰ�û��ķ���
	int IsFileOrDirOrFinger( ITrustZoneItem* pTrustItem);					//�жϷ����·��״̬
	void InitTable();																				//��ʼ��ҳ��
	
	void m_AddFile(LPCWSTR cstrFile,BOOL AbsolutePathType);		//����ļ��ķ���
	void m_AddDir(LPCWSTR	cstrDir);												//���Ŀ¼�ķ���
	void m_AddExt(LPCWSTR	cstrExt);												//���Ext�ķ���
	void m_DelPathAndDir();																//ɾ���б����ļ���Ŀ¼�ķ���
	void m_DelExt(ITrustRegion* pTrust);											//ɾ���б���Ext�ķ���
	void m_UpdateFileAndDirList();														//�����б����ļ���Ŀ¼�ķ���
	void m_UpdateExtList();																//�����б���Ext�ķ���

	QString  ToolTipWrap(QString strSrc,int width);							//tooltip��ʽ


	
public slots:
	void AddFileButton();	//����ļ���ť�ۺ���
	void AddDirButton();		//���Ŀ¼��ť�ۺ���
	void AddExtButton();		//�����չ����ť�ۺ���
	void SetAllCheckState(BOOL states);	//��ͷȫѡ�ۺ���
	void CheckTableChange(int row, int col);	//�������ѡ��״̬�۵ĺ���
	void DelPathAndDir();		//�ļ���Ŀ¼������ɾ����ť�ۺ���
	void DelExtButton();			//�ļ���չ��������ɾ����ť�ۺ���
	void isbutton5able(int row ,int col);	//�ļ�������ɾ����ť״̬�ۺ���


signals:
		void ChangeHeaderStates(bool type);//��ѡ��ѡ��仯�ź�

private:
	Ui::QAXSafeCheckerTrustDlgClass ui;


protected:
	void closeEvent(QCloseEvent *event);  											//�ر�ʱ�����Ĳۺ���




private:
	void initButton();																			//��ʼ����ť
	void initTitleBar();																			//��ʼ������
	//void paintEvent(QPaintEvent *event);											//��д��ͼ�¼�
	//void loadStyleSheet(const QString &sheetName);						//Ԥ������qss����

//private slots:
//	void onButtonMinClicked();															//��С���ۺ���
//	void onButtonRestoreClicked();														//��ԭ���ۺ���
//	void onButtonMaxClicked();															//��󻯲ۺ���
//	void onButtonCloseClicked();														//�رհ�ť�ۺ���

protected:
	MyTitleBar* m_titleBar;
	

};


#endif // QAXSAFECHECKERTRUST_H

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
	HMODULE  hDll;	//加载的dll句柄
	MyQHeaderView* myHeader; //重写表格的表头类
	ITrustRegion* pTrust;	//加载的dll中的类对象指针
	CString CStrbuffer; //一个公共的CString缓冲变量
	QString m_QStrFile;	//存放添加文件时的文件路径
	QString m_QStrDir;	//存放添加目录时的目录路径
	QString m_QStrExt;	//存放Ext扩展名
	int m_UserType;		//当前登录的用户的权限组(用户/管理员)
	ITrustZoneItem* m_pTrustItem;		//接口中文件和目录对象指针
	ITrustRegion* m_pTrustRegion;		//接口中Ext对象指针
	int ExtSelectRow;	//Ext列表中当前选择的行


	QAXSafeCheckerTrustDlg(QWidget *parent = 0);
	static QAXSafeCheckerTrustDlg* CreateInstance(QWidget* parent = Q_NULLPTR);	//返回子窗口对象
	BOOL  QAXSafeCheckerTrustDlg::CreatePage();
	BOOL   IsAdmin();																			//判断当前用户的方法
	int IsFileOrDirOrFinger( ITrustZoneItem* pTrustItem);					//判断放入的路径状态
	void InitTable();																				//初始化页面
	
	void m_AddFile(LPCWSTR cstrFile,BOOL AbsolutePathType);		//添加文件的方法
	void m_AddDir(LPCWSTR	cstrDir);												//添加目录的方法
	void m_AddExt(LPCWSTR	cstrExt);												//添加Ext的方法
	void m_DelPathAndDir();																//删除列表中文件及目录的方法
	void m_DelExt(ITrustRegion* pTrust);											//删除列表中Ext的方法
	void m_UpdateFileAndDirList();														//更新列表中文件及目录的方法
	void m_UpdateExtList();																//更新列表中Ext的方法

	QString  ToolTipWrap(QString strSrc,int width);							//tooltip格式


	
public slots:
	void AddFileButton();	//添加文件按钮槽函数
	void AddDirButton();		//添加目录按钮槽函数
	void AddExtButton();		//添加扩展名按钮槽函数
	void SetAllCheckState(BOOL states);	//表头全选槽函数
	void CheckTableChange(int row, int col);	//检查表格中选择状态槽的函数
	void DelPathAndDir();		//文件及目录白名单删除按钮槽函数
	void DelExtButton();			//文件扩展名白名单删除按钮槽函数
	void isbutton5able(int row ,int col);	//文件白名单删除按钮状态槽函数


signals:
		void ChangeHeaderStates(bool type);//复选框选择变化信号

private:
	Ui::QAXSafeCheckerTrustDlgClass ui;


protected:
	void closeEvent(QCloseEvent *event);  											//关闭时触发的槽函数




private:
	void initButton();																			//初始化按钮
	void initTitleBar();																			//初始化标题
	//void paintEvent(QPaintEvent *event);											//重写绘图事件
	//void loadStyleSheet(const QString &sheetName);						//预留加载qss函数

//private slots:
//	void onButtonMinClicked();															//最小化槽函数
//	void onButtonRestoreClicked();														//还原键槽函数
//	void onButtonMaxClicked();															//最大化槽函数
//	void onButtonCloseClicked();														//关闭按钮槽函数

protected:
	MyTitleBar* m_titleBar;
	

};


#endif // QAXSAFECHECKERTRUST_H

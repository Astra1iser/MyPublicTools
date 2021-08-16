#include "QAXSafeCheckerTrustDlg.h"

using namespace std;

//获取天擎安装路径
CString  GetQAXDLLPath(CString dllpath)
{
	TCHAR szPath[MAX_PATH] = { 0 };

#ifdef _WIN64
	static CONST TCHAR App_Paths_360Safe[] = TEXT("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\360safe.exe");
#else
	static CONST TCHAR App_Paths_360Safe[] = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\360safe.exe");
#endif

	static CONST TCHAR Value[] = TEXT("Path");
	HKEY hkey = NULL;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, App_Paths_360Safe,
		0, KEY_READ | KEY_QUERY_VALUE, &hkey);
	if (lRet == ERROR_SUCCESS)
	{
		DWORD dwLen = MAX_PATH * sizeof(TCHAR);
		DWORD access = REG_SZ;
		::RegQueryValueEx(hkey, Value, NULL, &access, (LPBYTE)szPath, &dwLen);
		::RegCloseKey(hkey);
	}

	return (CString)(szPath+dllpath);
}

//判断当前登录的账户是普通用户还是管理员
BOOL  QAXSafeCheckerTrustDlg::IsAdmin()  {  

	HANDLE		hToken;  
	DWORD		dwStatus;  
	DWORD		dwAccessMask;  
	DWORD		dwAccessDesired;  
	DWORD		dwACLSize;  
	DWORD		dwStructureSize	=	sizeof(PRIVILEGE_SET);  
	PACL			pACL					=	NULL;  
	PSID				psidAdmin			=	NULL;  
	BOOL			bReturn				=	FALSE;  

	PRIVILEGE_SET				ps;  
	GENERIC_MAPPING		GenericMapping;  

	PSECURITY_DESCRIPTOR			psdAdmin					=	NULL;  
	SID_IDENTIFIER_AUTHORITY	SystemSidAuthority	=	SECURITY_NT_AUTHORITY;  

	__try   
	{  
		ImpersonateSelf(SecurityImpersonation);  

		if   (!OpenThreadToken(GetCurrentThread(),   TOKEN_QUERY,   FALSE,    &hToken))   
		{  
				if   (GetLastError()   !=   ERROR_NO_TOKEN)  
					__leave;  
				if   (!OpenProcessToken(GetCurrentProcess(),   TOKEN_QUERY,    
					&hToken))  
					__leave;  
		}  

		if   (!AllocateAndInitializeSid(&SystemSidAuthority,   2,    SECURITY_BUILTIN_DOMAIN_RID,   DOMAIN_ALIAS_RID_ADMINS,  0,   0,   0,   0,   0,   0,   &psidAdmin))  
			__leave;  

		psdAdmin   =   LocalAlloc(LPTR,   SECURITY_DESCRIPTOR_MIN_LENGTH);  

		if   (psdAdmin   ==   NULL)  
			__leave;  

		if   (!InitializeSecurityDescriptor(psdAdmin,  
			SECURITY_DESCRIPTOR_REVISION))  
			__leave;  

		dwACLSize   =   sizeof(ACL)   +   sizeof(ACCESS_ALLOWED_ACE)   +  
			GetLengthSid(psidAdmin)   -   sizeof(DWORD);  

		pACL   =   (PACL)LocalAlloc(LPTR,   dwACLSize);  
		if   (pACL   ==   NULL)  
			__leave;  

		if   (!InitializeAcl(pACL,   dwACLSize,   ACL_REVISION2))  
			__leave;  

		dwAccessMask=   ACCESS_READ   |   ACCESS_WRITE;  

		if   (!AddAccessAllowedAce(pACL,   ACL_REVISION2,  
			dwAccessMask,   psidAdmin))  
			__leave;  

		if   (!SetSecurityDescriptorDacl(psdAdmin,   TRUE,   pACL,   FALSE))  
			__leave;  

		SetSecurityDescriptorGroup(psdAdmin,   psidAdmin,   FALSE);  
		SetSecurityDescriptorOwner(psdAdmin,   psidAdmin,   FALSE);  

		if   (!IsValidSecurityDescriptor(psdAdmin))  
			__leave;  

		dwAccessDesired							=	ACCESS_READ;  
		GenericMapping.GenericRead			=	ACCESS_READ;  
		GenericMapping.GenericWrite		=	ACCESS_WRITE;  
		GenericMapping.GenericExecute	=	0;  
		GenericMapping.GenericAll			=	ACCESS_READ   |   ACCESS_WRITE;  

		if   (!AccessCheck(psdAdmin,   hToken,   dwAccessDesired,    &GenericMapping,   &ps,   &dwStructureSize,   &dwStatus,    &bReturn))   
		{  
				//cout<<GetLastError();	//这里AccessCheck()报错了
				__leave;  
		}  

		RevertToSelf();  

	}   __finally   {  

		if   (pACL)				LocalFree(pACL);  
		if   (psdAdmin)	LocalFree(psdAdmin);      
		if   (psidAdmin)	FreeSid(psidAdmin);  
	}  

	return   bReturn;  
}  

QAXSafeCheckerTrustDlg* QAXSafeCheckerTrustDlg::CreateInstance(QWidget* parent)
{
	return new QAXSafeCheckerTrustDlg(parent);		
}

QAXSafeCheckerTrustDlg::QAXSafeCheckerTrustDlg(QWidget *parent)
	: QDialog(parent)
{
	hDll = LoadLibrary(GetQAXDLLPath(L"\\deepscan\\TrustRegion.dll"));
	if(NULL == hDll)
	{

	}
	GetTrustRegionProc pFun  = (GetTrustRegionProc)GetProcAddress(hDll, "GetTrustRegion");
	pTrust = pFun();
	pTrust->Init();
	m_UserType = IsAdmin()?admin_trust:user_trust;
	m_pTrustItem = NULL;
	m_pTrustRegion=NULL;

}

//创建子页面
BOOL  QAXSafeCheckerTrustDlg::CreatePage()
{
		ui.setupUi(this);
	

	//Qt::WindowFlags flags=Qt::Dialog;
	//flags |=Qt::WindowCloseButtonHint;
	//setWindowFlags(flags);

	initTitleBar();
	initButton();
	InitTable();
	//this->setWindowTitle(QString::fromLocal8Bit("信任区"));
	//this->setModal(true);
	////this->setAttribute(Qt::WA_DeleteOnClose);
	//this->exec();
	this->setWindowModality(Qt::WindowModal);
	this->setWindowIcon(QIcon("image/QAXico.png"));
	this->show();

	return TRUE;
}

void QAXSafeCheckerTrustDlg::InitTable()
{

	myHeader = new MyQHeaderView(Qt::Horizontal, ui.FileAndDirListTable);

	myHeader->setStretchLastSection(true);
	//myHeader->setStyleSheet("alignment: left;");
	//myHeader->setStyleSheet("alignment: right");
	//ui.FileAndDirListTable->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignHCenter);
	QObject::connect(myHeader, &MyQHeaderView::headCheckBoxToggled, this, &QAXSafeCheckerTrustDlg::SetAllCheckState);
	QObject::connect(ui.FileAndDirListTable, SIGNAL(cellChanged(int,int)), this, SLOT(CheckTableChange(int, int)));
	QObject::connect(this,&QAXSafeCheckerTrustDlg::ChangeHeaderStates,myHeader,&MyQHeaderView::stdate);
	ui.FileAndDirListTable->setHorizontalHeader(myHeader);//设置表头
	ui.FileAndDirListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	//ui.FileAndDirListTable->setFrameShape(QFrame::NoFrame); //设置无边框
	ui.FileAndDirListTable->setShowGrid(false); //设置不显示格子线
	ui.FileAndDirListTable->verticalHeader()->setVisible(false); //设置垂直头不可见
	ui.FileAndDirListTable->horizontalHeader()->setHighlightSections(false); 
	ui.FileAndDirListTable->setFocusPolicy(Qt::NoFocus);
	//设置表头字体加粗
	QFont font = ui.FileAndDirListTable->horizontalHeader()->font();
	font.setBold(true);
	ui.FileAndDirListTable->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
	ui.FileAndDirListTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
	ui.FileAndDirListTable->setSelectionMode(QAbstractItemView::SingleSelection);	//设置选择行为时每次选择一行
	ui.FileAndDirListTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui.FileAndDirListTable->horizontalHeader()->resizeSection(0,20); //设置表头第一列的宽度为10
	ui.FileAndDirListTable->horizontalHeader()->resizeSection(1,420); //设置表头第二列的宽度为420
	ui.FileAndDirListTable->horizontalHeader()->resizeSection(2,120); //设置表头第二列的宽度为420
	ui.FileAndDirListTable->horizontalHeader()->setFixedHeight(25); //设置表头的高度
	ui.FileAndDirListTable->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
	//ui.FileAndDirListTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
	ui.FileAndDirListTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);//滚动按项移动






	QObject::connect(ui.ExtListTable,SIGNAL(cellClicked(int ,int)),this,SLOT(isbutton5able(int,int)));
	ui.ExtListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	//ui.ExtListTable->setFrameShape(QFrame::NoFrame); //设置无边框
	ui.ExtListTable->setShowGrid(false); //设置不显示格子线
	ui.ExtListTable->verticalHeader()->setVisible(false); //设置垂直头不可见
	ui.ExtListTable->horizontalHeader()->setHighlightSections(false); 
	ui.ExtListTable->setFocusPolicy(Qt::NoFocus);
	//设置表头字体加粗
	QFont font2 = ui.ExtListTable->horizontalHeader()->font();
	font2.setBold(true);
	ui.ExtListTable->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
	ui.ExtListTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
	ui.ExtListTable->setSelectionMode(QAbstractItemView::SingleSelection);	//设置选择行为时每次选择一行
	ui.ExtListTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui.ExtListTable->horizontalHeader()->resizeSection(0,440); //设置表头第一列的宽度为10
	ui.ExtListTable->horizontalHeader()->resizeSection(1,130); //设置表头第二列的宽度为420
	ui.ExtListTable->horizontalHeader()->setFixedHeight(25); //设置表头的高度
	ui.ExtListTable->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
	//ui.FileAndDirListTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
	//ui.ExtListTable->horizontalHeader()->setStyleSheet( "QHeaderView::section {background-color:white; border:none; border-right:1px solid gray;}");
	//ui.ExtListTable->horizontalHeader()->setHighlightSections(false);
	ui.ExtListTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);//滚动按项移动
	ui.ExtListTable->horizontalHeader()->setSelectionMode(QAbstractItemView::NoSelection);//不可选择


	if (pTrust->IsFolderDisabled())				//判断当前策略
	{
		ui.AddFileButton->setDisabled(true);
		ui.AddDirButton->setDisabled(true);
		ui.FileAndDirDelButton->setDisabled(true);
	}

	if (pTrust->IsExtDisabled())					//判断当前策略
	{
		ui.AddExtButton->setDisabled(true);
		ui.ExtDelButton->setDisabled(true);
	}

	m_UpdateFileAndDirList();
	m_UpdateExtList();
}

void QAXSafeCheckerTrustDlg::SetAllCheckState(BOOL states)
{
	if (!ui.FileAndDirListTable->rowCount())
	{
		ChangeHeaderStates(FALSE);
	}
	else
	{
		if (0==states)
		{

			for (int i=0;i<ui.FileAndDirListTable->rowCount();i++)
			{
				if (ui.FileAndDirListTable->item(i,0)->checkState() == Qt::Checked)
				{
					QTableWidgetItem *checkBox = new QTableWidgetItem();
					checkBox->setCheckState(Qt::Unchecked);
					ui.FileAndDirListTable->removeCellWidget(i,0);
					ui.FileAndDirListTable ->setItem(i, 0,checkBox);
				}
			}
		}

		if (1==states)
		{

			for (int i=0;i<ui.FileAndDirListTable->rowCount();i++)
			{
				if (ui.FileAndDirListTable->item(i,0)->checkState() == Qt::Unchecked)
				{
					QTableWidgetItem *checkBox = new QTableWidgetItem();
					checkBox->setCheckState(Qt::Checked);
					ui.FileAndDirListTable->removeCellWidget(i,0);
					ui.FileAndDirListTable ->setItem(i, 0,checkBox);
				}
			}
		}
	}

	/*for (int i=0;i<ui.FileAndDirListTable->rowCount();i++)
	{
	if (ui.FileAndDirListTable->item(i,0)->checkState() == Qt::Checked)
	{
	cout<<"当前状态"<<i<<endl;
	}

	}*/
	//QTableWidgetItem *checkBox = new QTableWidgetItem();

	//checkBox->setCheckState(Qt::Checked);
	//ui.FileAndDirListTable->removeCellWidget(0,0);
	//ui.FileAndDirListTable ->setItem(0, 0,checkBox);
	
}

void QAXSafeCheckerTrustDlg::CheckTableChange(int row, int col)
{

	if(ui.FileAndDirListTable->item(row, col)->checkState() == Qt::Unchecked) //bu选中
		{
			ChangeHeaderStates(FALSE);
			int a=0;
			for (int i=0;i<ui.FileAndDirListTable->rowCount();i++)
			{
				if (ui.FileAndDirListTable->item(i,0)->checkState() == Qt::Unchecked)
				{
					a++;
				}
			}
			if(ui.FileAndDirListTable->rowCount()==a)
			{
				ui.FileAndDirDelButton->setDisabled(TRUE);
			}
		}


	if(ui.FileAndDirListTable->item(row, col)->checkState() == Qt::Checked) //选中
	{
		ui.FileAndDirDelButton->setDisabled(FALSE);
		int a=0;
		for (int i=0;i<ui.FileAndDirListTable->rowCount();i++)
		{
			if (ui.FileAndDirListTable->item(i,0)->checkState() == Qt::Checked)
			{
				a++;
			}
		}
		if(ui.FileAndDirListTable->rowCount()==a)
		{
			ChangeHeaderStates(TRUE);
		}
		else
			ChangeHeaderStates(FALSE);
	}



				
}

void QAXSafeCheckerTrustDlg::isbutton5able(int row ,int col)
{
	ExtSelectRow=row;
	ui.ExtDelButton->setDisabled(FALSE);
}

void QAXSafeCheckerTrustDlg::closeEvent(QCloseEvent *event)  
{
	FreeLibrary(hDll);
	delete this;
}

void QAXSafeCheckerTrustDlg::AddFileButton()
{	
	m_QStrFile.clear();
	
	m_QStrFile = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("选择文件"),"/","All Files(*.*)",nullptr);

	string str = m_QStrFile.toLocal8Bit().data();
	CStrbuffer=str.c_str();
	if (!CStrbuffer.IsEmpty())
	{
		AddFileDlg* dlg_AddFile=AddFileDlg::CreateInstance(this);
		dlg_AddFile->CreatePage(m_QStrFile);
		if(TRUE == dlg_AddFile->m_Type)
		{
			m_AddFile(CStrbuffer,dlg_AddFile->m_AbsolutePathType);
			CStrbuffer.Empty();
			dlg_AddFile->deleteLater();
			dlg_AddFile = nullptr;
		}
	}
}

void QAXSafeCheckerTrustDlg::m_AddFile(LPCWSTR cstrFile,BOOL AbsolutePathType)
{
	DWORD result;
	result=pTrust->AddPath(cstrFile, AbsolutePathType,m_UserType);	//添加文件接口

	//result:0:成功 1:已存在 2:无权限
	switch(result)
	{
		case 0:
			{
			m_UpdateFileAndDirList();
			}
			break;
		case 1:
			{
				QString message;
				TrustErrMsgDlg* dlg_TrustErrMsg=TrustErrMsgDlg::CreateInstance(this);
				message=QString("%1").arg(m_QStrFile);
				if (FALSE==AbsolutePathType)
					dlg_TrustErrMsg->CreatePage(message,1);
				else
					dlg_TrustErrMsg->CreatePage(message,2);
				dlg_TrustErrMsg->deleteLater();
				dlg_TrustErrMsg = nullptr;
			}
			break;
		case 2:
			{
				QString message;
				TrustErrMsgDlg*  dlg_TrustErrMsg=TrustErrMsgDlg::CreateInstance(this);
				message=QString("%1 %2 %3").arg(QString::fromLocal8Bit("错误代码:")).arg(result).arg(QString::fromLocal8Bit("权限不足"));
				dlg_TrustErrMsg->CreatePage(message,0);
				dlg_TrustErrMsg->deleteLater();
				dlg_TrustErrMsg = nullptr;
			}
			break;
	}
		
}

void QAXSafeCheckerTrustDlg::AddDirButton()
{
	m_QStrDir.clear();
	m_QStrDir=QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择路径"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	string str = m_QStrDir.toLocal8Bit().data();
	CStrbuffer=str.c_str();

	if (!CStrbuffer.IsEmpty())
	{
		AddDirDlg* dlg_AddDir=AddDirDlg::CreateInstance(this);
		dlg_AddDir->CreatePage(m_QStrDir);
		if (TRUE==dlg_AddDir->m_Type)
		{
			m_AddDir(CStrbuffer);
			CStrbuffer.Empty();
			dlg_AddDir->deleteLater();
			dlg_AddDir = nullptr;
		}
	}

}

void QAXSafeCheckerTrustDlg::m_AddDir(LPCWSTR cstrDir)
{
	DWORD result;
	result=pTrust->AddDir(cstrDir,m_UserType);

	//结果 0:成功 1:目录已存在 2:无权限 3:路径不在安全范围内
	switch(result)
	{
	case 0:
		{
			m_UpdateFileAndDirList();
		}
		break;

	case 1:
		{
			QString message;
			TrustErrMsgDlg* dlg_TrustErrMsg=TrustErrMsgDlg::CreateInstance(this);
			message=QString("%1").arg(m_QStrDir);
			dlg_TrustErrMsg->CreatePage(message,3);
			dlg_TrustErrMsg->deleteLater();
			dlg_TrustErrMsg = nullptr;
		}
		break;
	case 2:
		{
			QString message;
			TrustErrMsgDlg* dlg_TrustErrMsg=TrustErrMsgDlg::CreateInstance(this);
			message=QString("%1 %2 %3").arg(QString::fromLocal8Bit("错误代码:")).arg(result).arg(QString::fromLocal8Bit("权限不足"));
			dlg_TrustErrMsg->CreatePage(message,0);
			dlg_TrustErrMsg->deleteLater();
			dlg_TrustErrMsg = nullptr;
		}
		break;
	case 3:
		{
			QString message;
			TrustErrMsgDlg* dlg_TrustErrMsg=TrustErrMsgDlg::CreateInstance(this);
			message=QString("%1 %2").arg(QString::fromLocal8Bit("此目录不允许添加到信任列表.")).arg("出于系统安全考虑,信任列表中不允许添加整个磁盘,以及其他木马经常藏匿的系统目录");
			dlg_TrustErrMsg->CreatePage(message,0);
			dlg_TrustErrMsg->deleteLater();
			dlg_TrustErrMsg = nullptr;
		}
		break;
	}


}

void QAXSafeCheckerTrustDlg::AddExtButton()
{
		AddExtDlg* dlg_AddExt=AddExtDlg::CreateInstance(this);
		dlg_AddExt->CreatePage();

		m_QStrExt=dlg_AddExt->m_ExtName;
		string str = m_QStrExt.toLocal8Bit().data();
		CStrbuffer=str.c_str();

		if (!CStrbuffer.IsEmpty())
		{
			m_AddExt(CStrbuffer);
			CStrbuffer.Empty();
			dlg_AddExt->deleteLater();
			dlg_AddExt = nullptr;
		}

}

void QAXSafeCheckerTrustDlg::m_AddExt(LPCWSTR cstrExt)
{
	DWORD result;
	result=pTrust->AddExt(cstrExt , m_UserType);

	//结果 0:成功 1:已存在 2.无权限
	switch(result)
	{
	case 0:
		{
			m_UpdateExtList();
		}
		break;

	case 1:
		{
			QString message;
			TrustErrMsgDlg* dlg_TrustErrMsg=TrustErrMsgDlg::CreateInstance(this);
			message=QString("%1").arg(m_QStrExt);
			dlg_TrustErrMsg->CreatePage(message,4);
			dlg_TrustErrMsg->deleteLater();
			dlg_TrustErrMsg = nullptr;
		}
		break;
	case 2:
		{
			QString message;
			TrustErrMsgDlg* dlg_TrustErrMsg=TrustErrMsgDlg::CreateInstance(this);
			message=QString("%1 %2 %3").arg(QString::fromLocal8Bit("未知错误代码:")).arg(result).arg(QString::fromLocal8Bit("权限不足"));
			dlg_TrustErrMsg->CreatePage(message,0);
			dlg_TrustErrMsg->deleteLater();
			dlg_TrustErrMsg = nullptr;
		}
		break;
	}
}

int QAXSafeCheckerTrustDlg:: IsFileOrDirOrFinger( ITrustZoneItem* m_pTrustItem)
{
	//判断是指纹返回0
	//判断是存在的文件返回1
	//判断是不存在的文件返回3
	//判断是存在的目录返回2
	//判断是不存在的目录返回4
	//判断不出来返回5

	QString buf=QString::fromUtf16(reinterpret_cast<ushort*>(m_pTrustItem->GetFileMD5()));

	if ("00000000000000000000000000000000" != buf)
	{
		//return "文件指纹";
		return 0;
	}

	CComVariant vt;

	//判断是文件路径
	if(m_pTrustItem->GetProperty(L"trust_path", vt) == S_OK && V_VT(&vt) == VT_BOOL)
	{
		if(V_BOOL(&vt) == VARIANT_TRUE)
		{
			struct stat s;
			_bstr_t b = m_pTrustItem->GetLocation();
			char* lpszText2 = b;
			if (stat(lpszText2,&s)==0)
				return 1;
			else
				return 3;
		}
	}

	//判断是目录
	if(m_pTrustItem->GetProperty(L"trust_dir", vt) == S_OK && V_VT(&vt) == VT_BOOL)
	{
		if(V_BOOL(&vt) == VARIANT_TRUE)
		{
			struct stat s;
			_bstr_t b = m_pTrustItem->GetLocation();
			char* lpszText2 = b;
			if (stat(lpszText2,&s)==0)
				return 2;
			else
				return 4;
		}
	}


	//struct stat s;
	//_bstr_t b = m_pTrustItem->GetLocation();
	//char* lpszText2 = b;
	//if (stat(lpszText2,&s)==0)
	//{
	//	if(s.st_mode & S_IFDIR)
	//	{
	//		//return "目录";
	//		return 1;
	//	}else if (s.st_mode & S_IFREG){
	//		//return "文件";
	//		return 2;
	//	}else{
	//		//return "获取失败";
	//		return 3;
	//	}
	//}else{
	//	//return "位置或文件不存在";
	//	return 4;
	//}

	return 5;
}

void QAXSafeCheckerTrustDlg::DelPathAndDir()
{
	QString message;
	DelWarningDlg*  dlg_DelWarning=DelWarningDlg::CreateInstance(this);
	message=QString("%1").arg(QString::fromLocal8Bit("您确定移除所有已选定的项目吗?"));
	 dlg_DelWarning->CreatePage(message);
	if(TRUE== dlg_DelWarning->m_type)
		{
			m_DelPathAndDir();
			dlg_DelWarning->deleteLater();
			dlg_DelWarning = nullptr;
		}
}

void QAXSafeCheckerTrustDlg:: m_DelPathAndDir()
{
	QString QS_strType;
	QString QS_delpath;
	QString QS_delMd5;
	for(int i=0;i<ui.FileAndDirListTable->rowCount();i++)
	{
		if (ui.FileAndDirListTable->item(i,0)->checkState() == Qt::Checked)
		{
			QS_strType=ui.FileAndDirListTable->item(i,2)->text();
			string str = QS_strType.toLocal8Bit().data();
			CString buffer=str.c_str();
			//buffer是 "分类"

			if (buffer==L"文件路径"||buffer==L"路径不存在")
			{
				m_pTrustItem = (ITrustZoneItem*)pTrust->GetTrusItem((ui.FileAndDirListTable->rowCount()-1)-i);
				pTrust->DelPath(m_pTrustItem->GetName(),TRUE,NULL);
				m_pTrustItem=nullptr;
				continue;
			}

			if (buffer==L"文件指纹")
			{
				m_pTrustItem = (ITrustZoneItem*)pTrust->GetTrusItem((ui.FileAndDirListTable->rowCount()-1)-i);
				pTrust->DelPath(m_pTrustItem->GetName(),FALSE,m_pTrustItem->GetFileMD5());
				m_pTrustItem=nullptr;
				continue;
			}

			if (buffer==L"目录"||buffer==L"目录不存在")
			{
				m_pTrustItem = (ITrustZoneItem*)pTrust->GetTrusItem((ui.FileAndDirListTable->rowCount()-1)-i);
				pTrust->DelDir(m_pTrustItem->GetName());
				m_pTrustItem=nullptr;
				continue;;
			}
		}
	}
m_UpdateFileAndDirList();
}

void QAXSafeCheckerTrustDlg::DelExtButton()
{
	QString message;
	DelWarningDlg*  dlg_DelWarning=DelWarningDlg::CreateInstance(this);
	message=QString("%1").arg(QString::fromLocal8Bit("您确定移除所选扩展名白名单吗?"));
	 dlg_DelWarning->CreatePage(message);
	if(TRUE== dlg_DelWarning->m_type)
	{
		m_DelExt(pTrust);
		dlg_DelWarning->deleteLater();
		dlg_DelWarning = nullptr;
	}

}

void QAXSafeCheckerTrustDlg::m_DelExt(ITrustRegion* pTrust)
{
	TrustExtInfo info;
	(ITrustRegion*)pTrust->GetExtInfo(((ui.ExtListTable->rowCount()-1)-ExtSelectRow),info);
	pTrust->DelExt(info.szExtName);
	m_UpdateExtList();
}

void QAXSafeCheckerTrustDlg::m_UpdateFileAndDirList()
{

	pTrust->PushChange();		//先提交后重载
	pTrust->Reload();

	QFontMetrics fontWidth(ui.FileAndDirListTable->font()); //获取字体大小
	ui.FileAndDirListTable->setRowCount(0);							//将行置为行首
	//ui.FileAndDirListTable->clearContents();
	//multimap<ULONG64, ITrustZoneItem* >	Items ;
	const int nCount = (int)pTrust->GetTrustItemCount();
	//wcout<<"当前有"<<nCount<<"个数据"<<endl;
	//ui.FileAndDirListTable->insertRow(nCount); //插入新行
	int i = 0;
	m_pTrustItem = NULL; 
	for(i = nCount - 1; i >= 0; i --)		//倒序插入,正序显示
	{
		m_pTrustItem = (ITrustZoneItem*)pTrust->GetTrusItem(i);
		int rowcount =ui.FileAndDirListTable->rowCount();
		ui.FileAndDirListTable->insertRow(rowcount);
		if(m_pTrustItem)
		{
			//写复选框
			QTableWidgetItem *CheckBox = new QTableWidgetItem();
			CheckBox->setCheckState(Qt::Unchecked);
			ui.FileAndDirListTable ->setItem(rowcount, 0,CheckBox);

			QString QStrbuffer=QString::fromUtf16(reinterpret_cast<ushort*>(m_pTrustItem->GetLocation()));
			//ui.FileAndDirListTable->setItem(rowcount,1,new QTableWidgetItem(QString::fromUtf16(reinterpret_cast<ushort*>(m_pTrustItem->GetLocation()))));

			//设置表格元素显示及省略号位置及其tips
			QString elideNote = fontWidth.elidedText(QStrbuffer, Qt::ElideMiddle, 420);
			ui.FileAndDirListTable->setItem(rowcount,1,new QTableWidgetItem(elideNote));
			ui.FileAndDirListTable->item(rowcount,1)->setToolTip(ToolTipWrap(QStrbuffer,400));//设置tooltip

			//判断文件类型
			switch(IsFileOrDirOrFinger(m_pTrustItem))
			{
			case 0:
				ui.FileAndDirListTable->setItem(rowcount,2,new QTableWidgetItem(QString::fromLocal8Bit("文件指纹")));
				break;
			case 1:
				ui.FileAndDirListTable->setItem(rowcount,2,new QTableWidgetItem(QString::fromLocal8Bit("文件路径")));
				break;
			case 2:
				ui.FileAndDirListTable->setItem(rowcount,2,new QTableWidgetItem(QString::fromLocal8Bit("目录")));
				break;
			case 3:
				ui.FileAndDirListTable->setItem(rowcount,2,new QTableWidgetItem(QString::fromLocal8Bit("路径不存在")));
				break;
			case 4:
				ui.FileAndDirListTable->setItem(rowcount,2,new QTableWidgetItem(QString::fromLocal8Bit("目录不存在")));
				break;
			}
		}
		/*if (pTrust->IsFolderDisabled())
		{
		ui.FileAndDirListTable->item(rowcount,0)->setFlags(Qt::ItemIsEditable);
		}*/
	}

	//如果当前表是空的,那么将表头置为不可选
	if (!ui.FileAndDirListTable->rowCount())
	{
		ChangeHeaderStates(FALSE);
	}
	//每次更新完都将删除按钮置灰
	ui.FileAndDirDelButton->setDisabled(TRUE);

	//如果当前策略是禁用,那么锁定表格
	if (pTrust->IsFolderDisabled())
	{
		ui.FileAndDirListTable->setDisabled(TRUE);
	}
}

void QAXSafeCheckerTrustDlg::m_UpdateExtList()
{

	pTrust->PushChange();
	pTrust->Reload();
	ui.ExtListTable->setRowCount(0);
	ui.ExtListTable->clearContents();
	const int nCount = (int)pTrust->GetExtItemCount();
	int i = 0;
	for(i = nCount - 1; i >= 0; i --)
	{
		TrustExtInfo info;
		(ITrustRegion*)pTrust->GetExtInfo(i,info);
		int rowcount =ui.ExtListTable->rowCount();
		ui.ExtListTable->insertRow(rowcount);

		if(sizeof(info))
		{
			//插入扩展名
			std::wstring buffer = (LPCTSTR)CString(info.szExtName);
			QString QStrbuffer = QString::fromStdWString(buffer);
			ui.ExtListTable->setItem(rowcount,0,new QTableWidgetItem(QStrbuffer));

			//插入时间
			buffer= (LPCTSTR)CString(info.szExtTime);
			QStrbuffer=QString::fromStdWString(buffer);
			ui.ExtListTable->setItem(rowcount,1,new QTableWidgetItem(QStrbuffer));
		}
	}


	ui.ExtDelButton->setDisabled(TRUE);


	if (pTrust->IsExtDisabled())
	{
		ui.ExtListTable->setDisabled(TRUE);
	}

}

//设置tooltip格式
QString  QAXSafeCheckerTrustDlg:: ToolTipWrap(QString strSrc,int width) 
{ 
	QString result;
	QFontMetrics fm(fontMetrics());
	int textWidthInPxs = fm.width(strSrc);
	const int rear = strSrc.length();
	int pre = 0, vernier = 1;
	unsigned int pickUpWidthPxs = 0;
	QString pickUp;
	unsigned int curLen = 0;

	if (textWidthInPxs <= width)
	{
		result = strSrc;
		return result;
	}

	while (vernier <= rear)
	{
		curLen = vernier - pre;
		pickUp = strSrc.mid(pre, curLen);
		pickUpWidthPxs = fm.width(pickUp);
		if (pickUpWidthPxs >= width)
		{
			result += pickUp + "\n";  // 插入换行符，或者使用<br/>标签
			pre = vernier;
			pickUpWidthPxs = 0;
		}
		++vernier;
	}

	if (pickUpWidthPxs < width && !pickUp.isEmpty())
	{
		result += pickUp;
	}

	return result;
}










void QAXSafeCheckerTrustDlg ::initTitleBar()
{

	// FramelessWindowHint属性设置窗口去除边框;
	// WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	// 设置窗口背景透明;
	//this->setAttribute(Qt::WA_TranslucentBackground);


	m_titleBar = new MyTitleBar(this);
	m_titleBar->move(0, 0);
	m_titleBar->setTitleIcon("image/QAXico.png");
	m_titleBar->setTitleContent(QStringLiteral(" 信任区"),10);
	//m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setButtonType(MIN_MAX_BUTTON);
	m_titleBar->setTitleWidth(this->width());


	connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(m_titleBar->onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(m_titleBar->onButtonRestoreClicked()));
	connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(m_titleBar->onButtonMaxClicked()));
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(m_titleBar->onButtonCloseClicked()));
}

//void QAXSafeCheckerTrustDlg::paintEvent(QPaintEvent* event)
//{
//	//设置背景色;
//	QPainter painter(this);
//	QPainterPath pathBack;
//	pathBack.setFillRule(Qt::WindingFill);
//	pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
//	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
//	painter.fillPath(pathBack, QBrush(QColor(255, 255, 255)));
//
//	QWidget::paintEvent(event);                                                                                            
//}

//void QAXSafeCheckerTrustDlg::loadStyleSheet(const QString &sheetName)
//{
//	QFile file("image/" + sheetName + ".css");
//	file.open(QFile::ReadOnly);
//	if (file.isOpen())
//	{
//		QString styleSheet = this->styleSheet();
//		styleSheet += QLatin1String(file.readAll());
//		this->setStyleSheet(styleSheet);
//	}
//}

//void QAXSafeCheckerTrustDlg::onButtonMinClicked()
//{
//	showMinimized();
//}
//
//void QAXSafeCheckerTrustDlg::onButtonRestoreClicked()
//{
//	QPoint windowPos;
//	QSize windowSize;
//	m_titleBar->getRestoreInfo(windowPos, windowSize);
//	this->setGeometry(QRect(windowPos, windowSize));
//}
//
//void QAXSafeCheckerTrustDlg::onButtonMaxClicked()
//{
//	m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
//	QRect desktopRect = QApplication::desktop()->availableGeometry();
//	QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
//	setGeometry(FactRect);
//}
//
//void QAXSafeCheckerTrustDlg::onButtonCloseClicked()
//{
//	close();
//}







void QAXSafeCheckerTrustDlg::initButton()
{

	ui.AddFileButton->setStyleSheet("QPushButton{color:white;border:2px groove gray;border-radius:4px;padding:2px 4px;background-color: rgb(0, 120, 232);}");
	ui.AddFileButton->setStyleSheet("QPushButton{color:white;background-color: rgb(0, 120, 232);border:1px groove gray;border-radius:4px;padding:1px 4px;border-style: outset;}"
		"QPushButton:hover{background-color:rgb(0, 170, 232); color: white;}"
		"QPushButton:pressed{background-color:rgb(0, 60, 200);border-style: inset;}");

	ui.AddDirButton->setStyleSheet("QPushButton{color:white;border:2px groove gray;border-radius:4px;padding:2px 4px;background-color: rgb(0, 120, 232);}");
	ui.AddDirButton->setStyleSheet("QPushButton{color:white;background-color: rgb(0, 120, 232);border:1px groove gray;border-radius:4px;padding:1px 4px;border-style: outset;}"
		"QPushButton:hover{background-color:rgb(0, 170, 232); color: white;}"
		"QPushButton:pressed{background-color:rgb(0, 60, 200);border-style: inset;}");

	ui.AddExtButton->setStyleSheet("QPushButton{color:white;border:2px groove gray;border-radius:4px;padding:2px 4px;background-color: rgb(0, 120, 232);}");
	ui.AddExtButton->setStyleSheet("QPushButton{color:white;background-color: rgb(0, 120, 232);border:1px groove gray;border-radius:4px;padding:1px 4px;border-style: outset;}"
		"QPushButton:hover{background-color:rgb(0, 170, 232); color: white;}"
		"QPushButton:pressed{background-color:rgb(0, 60, 200);border-style: inset;}");

	ui.FileAndDirDelButton->setStyleSheet("QPushButton{border:2px groove gray;border-radius:4px;padding:2px 4px;background-color: rgb(255, 255, 255);}");
	ui.FileAndDirDelButton->setStyleSheet("QPushButton{background-color: rgb(225, 225, 225);border:1px groove gray;border-radius:4px;padding:1px 4px;border-style: outset;}"
		"QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
		"QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");

	ui.ExtDelButton->setStyleSheet("QPushButton{border:2px groove gray;border-radius:4px;padding:2px 4px;background-color: rgb(255, 255, 255);}");
	ui.ExtDelButton->setStyleSheet("QPushButton{background-color: rgb(225, 225, 225);border:1px groove gray;border-radius:4px;padding:1px 4px;border-style: outset;}"
		"QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
		"QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
}
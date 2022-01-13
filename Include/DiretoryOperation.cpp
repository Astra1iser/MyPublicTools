#pragma once
#include "stdafx.h"
#include "DiretoryOperation.h"
#include <SetupAPI.h>
#include <ShlObj.h>
#include <strsafe.h>

// �ж�Ŀ¼�Ƿ����
BOOL DirectoryOperation::FolderExists(CString strDir)
{
	DWORD attr;
	attr = GetFileAttributes(strDir);
	return (attr != (DWORD)(-1) ) &&
		( attr & FILE_ATTRIBUTE_DIRECTORY);
}
// ����Ŀ¼
BOOL DirectoryOperation::CreateDirRecursive(CString strDir)
{
	int len = strDir.GetLength();

	if (len < 2) 
		return false;

	if('\\' == strDir[len-1] || '/' == strDir[len-1])
	{
		strDir = strDir.Left(len-1);
		len = strDir.GetLength();
	}
	if (len <= 0) 
		return false;

	if (len <= 3)
	{
		if (FolderExists(strDir))//Ŀ¼�Ƿ����
			return true;
		else 
			return false;
	}
	if (FolderExists(strDir))
		return true;

	CString Parent;
	Parent = strDir.Left(strDir.ReverseFind('\\') );//��Ŀ¼

	CString Parent2;
	Parent2 = strDir.Left(strDir.ReverseFind('/') );//��Ŀ¼
	if(Parent.GetLength() <= 0 && Parent2.GetLength() <= 0)
		return false;

	BOOL Ret = CreateDirRecursive(Parent);//����Ŀ¼

	if(Ret)
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = 0;

		Ret = (CreateDirectory(strDir, &sa) == TRUE);
		return Ret;
	}
	else
		return FALSE;
}
// TopisDir����˼�Ǳ�ʾ strDir������һ��Ŀ¼������һ���ļ�����������ļ�������ֻ����������һ��
BOOL DirectoryOperation::CreateDirRecursive(CString strDir, BOOL TopisDir)
{//�����ļ�·��
	CString strTmpDir;
	CString strLastDir;
	for(int i=0;i<strDir.GetLength();i++)
	{
		TCHAR ch = strDir.GetAt(i);
		strTmpDir += ch;
		if(ch == _T('\\') || ch == _T('/'))
		{
			::CreateDirectory(strTmpDir, NULL);//��Ҫ��Ŀ¼��Ŀ¼һ��ֻ�ܴ���һ��
			strLastDir = strTmpDir;
		}
	}

	if(TopisDir)
	{
		::CreateDirectory(strDir, NULL);
		strLastDir = strDir;
	}

	return ::PathIsDirectory(strLastDir);
}
//get size of folder
DWORD64 DirectoryOperation::GetFolderSize(CString strFilePath, int &n_File)
{
	DWORD64 static size = 0;//��̬ ����Ҫ
	int static num = 0;
	CString strDir = strFilePath;
	int nDirPos = strFilePath.ReverseFind(L'\\');
	if (nDirPos != strDir.GetLength() - 1)//�ԡ�\����β
	{
		strDir+=L"\\";
	}
	strDir += L"*";

	WIN32_FIND_DATA fd;
	HANDLE hSearch = ::FindFirstFile(strDir.GetBuffer(0), &fd);
	if(hSearch == INVALID_HANDLE_VALUE)
		return false;

	do//�ݹ����
	{
		if ((_tcscmp(fd.cFileName,  _T(".")) == 0) || 
			(_tcscmp(fd.cFileName,  _T("..")) == 0))
			continue;

		CString strFileName = strFilePath;		
		int nPos = strFilePath.ReverseFind(L'\\');
		if (nPos != strFilePath.GetLength() - 1)
		{
			strFileName += L"\\";
		}
		strFileName += fd.cFileName;		
		if ((FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes) != 0)//�ж��Ƿ���Ŀ¼����
		{
			size += GetFolderSize(strFileName, num);					
		}
		else
		{
			size += fd.nFileSizeLow;
			num ++ ;
		}

	} while(::FindNextFile(hSearch, &fd));

	n_File = num;
	::FindClose(hSearch);
	return size;
}
BOOL DirectoryOperation::DeleteDirRecursive(CString strDir)
{
	if(PathFileExists(strDir))//����
		return ::RemoveDirectory(strDir);
	return FALSE;
}
BOOL DirectoryOperation::EntSafeCopyFile(LPCTSTR src, LPCTSTR dst)
{
	BOOL success = FALSE;
	HANDLE hSrc = INVALID_HANDLE_VALUE;
	HANDLE hDst = INVALID_HANDLE_VALUE;

	CString strDstFile = dst;
	CString strDstFileBak = strDstFile + _T(".entbak");
	do
	{
		//���Ŀ���ļ����ڵ��ļ��ж������ڣ��ȴ���һ�£���ȻCreateFile��ʧ��
		int nPos = strDstFile.ReverseFind(L'\\');
		if(-1 != nPos)
		{
			CString strDstDir = strDstFile.Left(nPos+1);
			if(!PathFileExists(strDstDir))
				::CreateDirectory(strDstDir,NULL);
		}

		//Ŀ���ļ��Ѿ����ڣ����ȱ���һ������
		if(::PathFileExists(strDstFile))
			MoveFileEx(strDstFile,strDstFileBak,MOVEFILE_REPLACE_EXISTING);

		// ��������CopyFile��ʵ�����Ŀ�걻ֻ������ʽ�򿪣�����Ȼʧ��
		// ���Ŀ�걻�����д��ʽ�򿪣��������Գɹ�
		hSrc = ::CreateFile(src, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hSrc == INVALID_HANDLE_VALUE) break;

		hDst = ::CreateFile(dst, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hDst == INVALID_HANDLE_VALUE) break;

		LARGE_INTEGER copy_size;
		copy_size.QuadPart = 0;
		BOOL ret = GetFileSizeEx(hSrc, &copy_size);
		if(ret == FALSE) break;

		LARGE_INTEGER pos;
		pos.QuadPart = 0;

		// 1. �ض�Ŀ���ļ�
		ret = ::SetEndOfFile(hDst);
		if(ret == FALSE) break;

		char buf[16*1024];

		for(;;)
		{
			DWORD nHave = 0;
			BOOL bok = ::ReadFile(hSrc, buf, sizeof(buf), &nHave, NULL);
			if(bok)
			{
				DWORD nhaveWrite = 0;
				bok = ::WriteFile(hDst, buf, nHave, &nhaveWrite, NULL);
				if(bok == FALSE || nHave != nhaveWrite)
					break;

				pos.QuadPart += nHave;
			}

			if(nHave == 0|| bok == FALSE)
				break;
		}
		::FlushFileBuffers(hDst);

		LARGE_INTEGER new_size;
		new_size.QuadPart = 0;
		ret = GetFileSizeEx(hDst, &new_size);
		if(ret == FALSE) break;

		if(new_size.QuadPart == copy_size.QuadPart)
			success = TRUE;

	}while(0);

	if(hSrc != INVALID_HANDLE_VALUE)
		::CloseHandle(hSrc);

	if(hDst != INVALID_HANDLE_VALUE)
		::CloseHandle(hDst);

	if(!success)
	{
		if(::PathFileExists(strDstFileBak))
			MoveFileEx(strDstFileBak,strDstFile,MOVEFILE_REPLACE_EXISTING);
	}
	else
		DeleteFile(strDstFileBak);

	return success;
}

BOOL DirectoryOperation::SafeMoveFile(CString src, CString dst, DWORD max_wait_second)
{
	BOOL ok = FALSE;
	for(DWORD i=0;i<(10*max_wait_second) + 1;i++)
	{
		BOOL del_old = FALSE;
		// ���֧�ֹ���ɾ�����Ϳ��Ը�������ʱ����ֱ��ɾ����ֱ��ɾ�����޷�����
		del_old = ::DeleteFile(dst + _T(".del"));
		if(::PathFileExists(dst))
		{
			// ����ļ����򿪣���ر�ʱϵͳ��ɾ��
			del_old = ::MoveFileEx(dst, dst + _T(".del"),MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
			del_old = ::DeleteFile(dst + _T(".del"));
			del_old = ::DeleteFile(dst);
		}

		ok = ::MoveFile(src, dst);
		if (ok)
		{
			MoveFileEx(dst + _T(".del"),NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
		}
		if(ok) break;

		Sleep(100);
	}

	return ok;
}

BOOL DirectoryOperation::GetFileVer(const CString& rFilePath, CString& rVer)
{
	//��ȡ�ļ��汾��Ϣ
	DWORD dwHandle,InfoSize;

	//���Ȼ�ð汾��Ϣ��Դ�ĳ���
	InfoSize = GetFileVersionInfoSize(rFilePath, &dwHandle);
	//���汾��Ϣ��Դ���뻺����
	if(InfoSize > 0)
	{
		TCHAR *InfoBuf = new TCHAR[InfoSize];
		GetFileVersionInfo(rFilePath,0,InfoSize,InfoBuf);

		//��������ļ�ʹ�õĴ���ҳ���ļ��汾
		unsigned int  cbTranslate = 0;
		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;

		VerQueryValue(InfoBuf, _T("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);

		// Read the file description for each language and code page.
		for( int i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
		{
			TCHAR SubBlock[200];
			_stprintf( SubBlock,
				_T("\\StringFileInfo\\%04x%04x\\FileVersion"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);

			void *lpBuffer=NULL;
			unsigned int dwBytes=0;
			VerQueryValue(InfoBuf,
				SubBlock,
				&lpBuffer,
				&dwBytes);
			CString strTemp = (TCHAR*)lpBuffer;
			rVer += strTemp;
		}
		delete [] InfoBuf ;
	}
	return TRUE;
}

BOOL DirectoryOperation::GetModuleFilePath(CString& strSubPath, CString& strOutFile)
{
	BOOL bResult	= FALSE;
	int nRetCode	= false;


//#ifdef WIN32
	TCHAR szAppPath[MAX_PATH] = _T("");
	LPTSTR pszPos = NULL;	
	nRetCode = GetModuleFileName(NULL, szAppPath, MAX_PATH);
	//QE_PROCESS_ERROR(nRetCode);

	pszPos = _tcsrchr(szAppPath, '\\');
	//QE_PROCESS_ERROR(pszPos != NULL);

	if (strSubPath.IsEmpty())
	{
		pszPos[0] = '\0';
		strOutFile = szAppPath;
	}
	else
	{
		pszPos[1] = '\0';
		strOutFile = szAppPath;
		strOutFile += strSubPath;
	}

//#else
//#endif
	bResult = TRUE;

	return bResult;
}

void DirectoryOperation::DeleteAllFileInDir(const CString &strFolder)
{//*ɾ������Ŀ¼�µ������ļ�
	CString strDir = strFolder;

	int nDirPos = strFolder.ReverseFind('\\');
	if (nDirPos != strDir.GetLength() - 1)
	{
		strDir+=_T("\\");
	}
	strDir += _T("*");

	WIN32_FIND_DATA fd;
	HANDLE hSearch = ::FindFirstFile(strDir.GetBuffer(0), &fd);
	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if ((_tcscmp(fd.cFileName,  _T(".")) == 0) || 
			(_tcscmp(fd.cFileName,  _T("..")) == 0))
			continue;

		CString strFileName = strFolder;
		int nPos = strFolder.ReverseFind('\\');
		if (nPos != strFolder.GetLength() - 1)
		{
			strFileName += _T("\\");
		}
		strFileName += fd.cFileName;

		if ((FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes) != 0)//*�ж��Ƿ���Ŀ¼����
		{
			DeleteAllFileInDir(strFileName);//*ɾ������Ŀ¼�µ������ļ������������ļ���
			RemoveDirectory(strFileName.GetBuffer(0));//*ɾ��Ŀ¼
		}
		else
		{
			DeleteFile(strFileName.GetBuffer(0));//*����Ŀ¼��ֱ��ɾ���ļ�
		}

	} while(::FindNextFile(hSearch, &fd));

	::FindClose(hSearch);
}

BOOL SafeCopyFile(CString src, CString dst)
{
	BOOL ok = FALSE;
	for(DWORD i=0;i< 10;i++)
	{
		BOOL del_old = FALSE;
		// ���֧�ֹ���ɾ�����Ϳ��Ը�������ʱ����ֱ��ɾ����ֱ��ɾ�����޷�����
		del_old = ::DeleteFile(dst + _T(".del"));
		if(::PathFileExists(dst))
		{
			// ����ļ����򿪣���ر�ʱϵͳ��ɾ��
			del_old = ::MoveFile(dst, dst + _T(".del"));
			del_old = ::DeleteFile(dst + _T(".del"));
			del_old = ::DeleteFile(dst);
		}

		ok = ::CopyFile(src, dst, FALSE);
		if(ok) break;

		Sleep(100);
	}

	return ok;
}

/****************************************************************************
*����һ���ļ����µ������ļ���Ŀ���ļ���*111
*strScrFolder\strDestFolder�ֱ�ΪԴ��Ŀ���ļ��У����ԡ�\'��β
*****************************************************************************/
BOOL DirectoryOperation::CopyAllFileInDir(const CString strScrFolder,const CString strDestFolder)
{//*ɾ������Ŀ¼�µ������ļ�
	CString strDir = strScrFolder;

	int nDirPos = strScrFolder.ReverseFind('\\');
	if (nDirPos != strDir.GetLength() - 1)
	{
		strDir += _T("\\");
	}
	strDir += _T("*");

	WIN32_FIND_DATA fd;
	HANDLE hSearch = ::FindFirstFile(strDir.GetBuffer(0), &fd);
	if(hSearch == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if ((_tcscmp(fd.cFileName,  _T(".")) == 0) || 
			(_tcscmp(fd.cFileName,  _T("..")) == 0))
			continue;

		CString strFileName = strScrFolder;
		CString strDestTem=strDestFolder;
		int nPos = strScrFolder.ReverseFind('\\');
		if (nPos != strScrFolder.GetLength() - 1)
		{
			strFileName += _T("\\");
		}
		strFileName += fd.cFileName;
		strDestTem+=fd.cFileName;
		if ((FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes) != 0)//*11�ж��Ƿ���Ŀ¼����
		{
			CreateDirRecursive(strDestTem, FALSE);//*11
			CopyAllFileInDir(strFileName,strDestTem);//			
		}
		else
		{
			if(-1 != strFileName.Find(_T("cache")))//�ڴ��ļ�����
				continue;
			BOOL bCopySuccess = FALSE;
			for(int i  = 0; i < 6; ++i)
			{
				if((::CopyFile(strFileName,strDestTem,FALSE) == FALSE)&&!SafeCopyFile(strFileName,strDestTem))
				{
					Sleep(100);
					continue;
				}
				bCopySuccess = TRUE;
				break;
			}
			if (!bCopySuccess)
			{
				WRITE_LOG(L"CopyAllFileInDir:nCopy failed %s->%s ��lasterror=%d", strFileName.GetBuffer(0),strDestTem.GetBuffer(0), GetLastError());
				::FindClose(hSearch);
				return FALSE;
			}
		}

	} while(::FindNextFile(hSearch, &fd));

	::FindClose(hSearch);
	return TRUE;
}

BOOL DirectoryOperation::CopyOneFile(const CString strScrFolder,const CString strDestFolder)
{
	BOOL bCopySuccess = FALSE;
	if (!PathFileExists(strScrFolder))
	{
		WRITE_LOG(_T("CopyOneFile():file:%s do not exist..."), strScrFolder);
	}
	for(int i  = 0; i < 6; ++i)
	{
		if((::CopyFile(strScrFolder,strDestFolder,FALSE) == FALSE)&&!SafeCopyFile(strScrFolder,strDestFolder))
		{
			Sleep(100);
			continue;
		}
		bCopySuccess = TRUE;
		break;
	}
	if(!bCopySuccess)
		WRITE_LOG(_T("CopyOneFile():%s->%s fail, last error=%d..."),strScrFolder, strDestFolder, GetLastError());
	return bCopySuccess;
}

BOOL DirectoryOperation::MakeCab(CString strCabFile, CString strFolder, std::list<CString> fileList)
{
	CCabinet cab;
	if(!cab.CreateCabinet (strCabFile))
	{
		WRITE_LOG(_T("����CAB�ļ�%sʧ�ܣ������˳���"), strCabFile);
		return FALSE;
	}
	
	for(std::list<CString>::iterator iter = fileList.begin(); iter != fileList.end(); iter++)
	{
		CString strFile = strFolder + *iter;
		try
		{	
			BOOL ok = cab.AddFile2Cab(strFile);
			if (!ok)
			{
				WRITE_LOG(_T("��CAB�ļ�%s�����ļ�%sʧ�ܣ������˳���"), strCabFile,  strFile);
				return FALSE;
			}
		}
		catch (...)
		{
			WRITE_LOG(_T("����CAB�ļ�%s�쳣�������˳���"), strCabFile);
			return FALSE;
		}
	}
	if (!cab.FlushCab())
	{
		WRITE_LOG(_T("FlushCab CAB�ļ�%sʧ�ܣ������˳���"), strCabFile);
		return FALSE;
	}
	return TRUE;
}

BOOL DirectoryOperation::MakeCab(CString strCabFile, CString strFolder)
{
	CCabinet cab;
	if(!cab.CreateCabinet (strCabFile))
	{
		WRITE_LOG(_T("����CAB�ļ�%sʧ�ܣ������˳���"), strCabFile);
		return FALSE;
	}

	CString strDir = strFolder;

	int nDirPos = strFolder.ReverseFind('\\');
	if (nDirPos != strDir.GetLength() - 1)
	{
		strDir += _T("\\");
	}
	strDir += _T("*");

	WIN32_FIND_DATA fd;
	HANDLE hSearch = ::FindFirstFile(strDir.GetBuffer(0), &fd);
	if(hSearch == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if ((_tcscmp(fd.cFileName,  _T(".")) == 0) || 
			(_tcscmp(fd.cFileName,  _T("..")) == 0))
			continue;

		CString strFileName = strFolder;
		strFileName += fd.cFileName;
		try
		{	
			BOOL ok = cab.AddFile2Cab(strFileName);
			if (!ok)
			{
				WRITE_LOG(_T("��CAB�ļ�%s�����ļ�%sʧ�ܣ������˳���"), strCabFile,  strFileName);
				return FALSE;
			}
		}
		catch (...)
		{
			WRITE_LOG(_T("����CAB�ļ�%s�쳣�������˳���"), strCabFile);
			return FALSE;
		}

	} while(::FindNextFile(hSearch, &fd));

	::FindClose(hSearch);

	if (!cab.FlushCab())
	{
		WRITE_LOG(_T("FlushCab CAB�ļ�%sʧ�ܣ������˳���"), strCabFile);
		return FALSE;
	}
	return TRUE;
}

typedef BOOL (WINAPI* PFNSetupIterateCabinet)(
	__in PCWSTR CabinetFile,
	__reserved DWORD Reserved,
	__in PSP_FILE_CALLBACK_W MsgHandler,
	__in PVOID Context
	);


static LRESULT WINAPI CabinetCallback ( PVOID Context, UINT Notification, UINT uParam1, UINT uParam2 )
{
	switch (Notification)
	{
		// SetupIterateCabinet is going to unpack a file and we have to tell 
		// it where it should place the file 
	case SPFILENOTIFY_FILEINCABINET: 
		{
			FILE_IN_CABINET_INFO * pInfo = (FILE_IN_CABINET_INFO *)uParam1;

			TCHAR szTargetName[MAX_PATH];
			if( PathCombine(szTargetName, (LPCTSTR)Context, pInfo->NameInCabinet) )
			{
				StringCbCopy(pInfo->FullTargetName, sizeof(pInfo->FullTargetName), szTargetName);

				if(PathRemoveFileSpec(szTargetName))
				{
					SHCreateDirectory(NULL, szTargetName);
					return FILEOP_DOIT;
				}
			}

			return FILEOP_ABORT;
		}

		// a file was extracted
	case SPFILENOTIFY_FILEEXTRACTED:
		{
			FILEPATHS * pPaths = (FILEPATHS *)uParam1;
			return pPaths->Win32Error;
		}

		// a new cabinet file is required; this notification should never
		// appear in our program, since all files have to be in a single
		// cabinet
	case SPFILENOTIFY_NEEDNEWCABINET:
		{
			return ERROR_INVALID_PARAMETER;
		}
	}
	return ERROR_SUCCESS;
}

BOOL DirectoryOperation::ExtractCab(CString strCabFile, CString strFolder)
{
	/*if( NULL == pszCabFile || NULL == pszDestPath )
		return FALSE;*/

	BOOL bret = FALSE;
	HMODULE hDll = LoadLibraryW(L"setupapi.dll");
	if(hDll)
	{
		PFNSetupIterateCabinet pFunc = (PFNSetupIterateCabinet)GetProcAddress(hDll, "SetupIterateCabinetW");
		if (pFunc)
		{
			bret = pFunc(strCabFile.GetBuffer(0), 0, (PSP_FILE_CALLBACK)CabinetCallback, (LPVOID)strFolder.GetBuffer(0) );
		}
		FreeLibrary(hDll);
	}

	return bret;
}

CString DirectoryOperation::GetTempFilePath()
{
	static CString strTempPath;
	do
	{
		if (strTempPath.IsEmpty())
		{
			WCHAR wszTempPath[MAX_PATH] = {0};
			DWORD dwLen = GetTempPath(MAX_PATH, wszTempPath);
			if(0 == dwLen)
			{
				WRITE_LOG(L"GetTempPath() error...return 0: %d", GetLastError());
				break;
			}
			strTempPath = wszTempPath;
			WRITE_LOG(L"GetTempPath() :1 %d- %s", dwLen, strTempPath.GetBuffer(0));

			if (strTempPath.IsEmpty())
			{
				WRITE_LOG(L"GetTempPath() :3 is NULL, default:c:\\temp\\360EntUpd\\");
				strTempPath = L"c:\\temp\\";
			}

			strTempPath.Append(L"360EntUpd\\");
		}
	} while (FALSE);

	if(!CreateDirRecursive(strTempPath))
	{
		WRITE_LOG(L"GetTempPath() CreateDirRecursive- %s fail...", strTempPath.GetBuffer(0));
		strTempPath = L"c:\\";
		return strTempPath;
	}
	WRITE_LOG(L"GetTempPath() : %s", strTempPath.GetBuffer(0));
	return strTempPath;
}

BOOL DirectoryOperation::RenameFile(const CString strSrc, const CString StrDes)
{//Ҫ���ж�һ���ն���û������ļ���û�о�ֱ�ӿ���
	char chScr[MAX_PATH] = {0}, chDes[MAX_PATH] = {0};
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, chScr, MAX_PATH, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, StrDes, -1, chDes, MAX_PATH, NULL, NULL);
	int nRet = rename(chScr, chDes);
	if (0 != nRet)
	{
		WRITE_LOG(_T("RenameFile():fail, path=%s ret=%d, lasterr=%d"), strSrc, nRet, GetLastError());
		// return FALSE;
	}
	return TRUE;
}
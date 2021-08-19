#include "stdafx.h"
#include <WinCrypt.h>
#include "FileOpHelper.h"
//#include "CodecHelper.h"

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib,"Version.lib") 

#ifndef WRITE_LOG
#define WRITE_LOG
#endif

BOOL FileOpHelper::GetFileMD5String(const CString& strFile, CString& strMD5Result, int nLen)
{
	strMD5Result.Empty();

	if (nLen > 128)
		return FALSE;

	BYTE byMD5[128] = { 0 };
	CodecHelper::EntMd5File(strFile, byMD5);

	//输出类似369e795b3001d7305ccce580c79b676b的MD5
	CString strTemp;
	for (int nIndex = 0; nIndex < nLen; nIndex++)
	{
		strTemp.Empty();
		strTemp.Format(_T("%02X"), byMD5[nIndex]);
		strMD5Result += strTemp;
	}

	return TRUE;
}

int FileOpHelper::GetFileVersionCommentInfo(LPCTSTR strFile, CString& strInfo)
{
	DWORD dwHandle = 0;
	DWORD dwVerSize = 0;

	TCHAR szVer[1024] = { 0 };
	TCHAR szVersionBuffer[1024 * 16] = { 0 };

	strInfo.Empty();
	dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
	if(dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer) - 1))
		return FALSE;

	if(!GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))
		return FALSE;

	UINT nQuerySize = 0;  
	LPVOID lpData = NULL;
	if(!VerQueryValue(szVersionBuffer,_T("\\VarFileInfo\\Translation"),&lpData,&nQuerySize))
		return FALSE;
	
	for(UINT i=0; i < nQuerySize / 4; i++)	//WORD wLanguage;WORD wCodePage共4个字节 
	{
		LPVOID lpInfo = NULL;
		UINT nInfoSize = 0;

		CString strQueryString;
		strQueryString.Format(_T("\\StringFileInfo\\%04x%04x\\Comments"),LOWORD(((DWORD*)lpData)[i]),HIWORD(((DWORD*)lpData)[i]));
		if(VerQueryValue(szVersionBuffer, strQueryString, &lpInfo, &nInfoSize) && lpInfo)
		{
			strInfo = (LPCTSTR)lpInfo;
			if(!strInfo.IsEmpty())
				break;
		}
	}

	return TRUE;
}

void FileOpHelper::GetFileVersionDetailInfo(LPCTSTR szFullPath,CString& strFileDesc,CString& strProductDesc,CString& strCompany)
{
	strFileDesc = _T("");
	strProductDesc = _T("");
	strCompany = _T("");

	DWORD dwHandle = 0,dwVerSize = 0;
	TCHAR szVersionBuffer[1024 * 16] = { 0 };

	dwVerSize = GetFileVersionInfoSize(szFullPath, &dwHandle);
	if(dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer) - 1))
		return;

	if(!GetFileVersionInfo(szFullPath, 0, dwVerSize, szVersionBuffer))
		return;

	UINT nQuerySize = 0;  
	LPVOID lpData = NULL;
	if(VerQueryValue(szVersionBuffer,_T("\\VarFileInfo\\Translation"),&lpData,&nQuerySize))
	{
		for(UINT i=0; i < nQuerySize / 4; i++)	//WORD wLanguage;WORD wCodePage共4个字节 
		{
			LPVOID lpInfo = NULL;
			UINT nInfoSize = 0;

			CString strQueryString;
			strQueryString.Format(_T("\\StringFileInfo\\%04x%04x\\FileDescription"),LOWORD(((DWORD*)lpData)[i]),HIWORD(((DWORD*)lpData)[i]));
			if(VerQueryValue(szVersionBuffer, strQueryString, &lpInfo, &nInfoSize) && lpInfo)
				strFileDesc = (LPCTSTR)lpInfo;

			lpInfo = NULL;
			nInfoSize = 0;
			strQueryString.Format(_T("\\StringFileInfo\\%04x%04x\\ProductName"),LOWORD(((DWORD*)lpData)[i]),HIWORD(((DWORD*)lpData)[i]));
			if(VerQueryValue(szVersionBuffer, strQueryString, &lpInfo, &nInfoSize) && lpInfo)
				strProductDesc = (LPCTSTR)lpInfo;

			lpInfo = NULL;
			nInfoSize = 0;
			strQueryString.Format(_T("\\StringFileInfo\\%04x%04x\\CompanyName"),LOWORD(((DWORD*)lpData)[i]),HIWORD(((DWORD*)lpData)[i]));
			if(VerQueryValue(szVersionBuffer, strQueryString, &lpInfo, &nInfoSize) && lpInfo)
				strCompany = (LPCTSTR)lpInfo;
		}
	}
}

int FileOpHelper::GetPEFileVer(const CString& strFile, DWORD& dwHight, DWORD& dwLow)
{
	DWORD dwVerSize;
	DWORD dwHandle;

	TCHAR  szVersionBuffer[1024 * 16] = { 0 };
	TCHAR szVer[1024] = { 0 };

	dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
	if( dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer) - 1))
		return FALSE;

	if(!GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))
		return FALSE;

	unsigned int nInfoLen = 0;
	VS_FIXEDFILEINFO* pInfo = NULL;

	if(!VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))
		return FALSE;

	dwHight = pInfo->dwFileVersionMS;
	dwLow = pInfo->dwFileVersionLS;
	return TRUE;
}

VOID FileOpHelper::VersionStringToNum(LPCTSTR pVersion, DWORD *pMS, DWORD *pLS)
{
	#define MAX_VERSION_LEN 255
	DWORD dwMS = 0;
	DWORD dwLS = 0;
	do 
	{
		if(wcslen(pVersion) > MAX_VERSION_LEN)
			break;

		TCHAR szVersion[MAX_VERSION_LEN + 1] = {0};
		wcscpy(szVersion, pVersion);

		TCHAR *token = wcstok( szVersion, _T(".") );

		if(token)
		{
			dwMS += _wtoi(token) << 16;
			token = wcstok( NULL, _T(".") );
			if(token)
			{
				dwMS += _wtoi(token);
				token = wcstok( NULL, _T(".") );
				if(token)
				{
					dwLS += _wtoi(token) << 16;
					token = wcstok( NULL, _T(".") );
					if(token)
					{
						dwLS += _wtoi(token);
						token = wcstok( NULL, _T(".") );
					}
				}
			}
		}
		ATLASSERT(!token);
	} while (false);

	if(pMS)
		*pMS = dwMS;
	if(pLS)
		*pLS = dwLS;
}

CString FileOpHelper::GetExeVer(const CString& strFile)
{
	CString strVer;
	GetPEFileVer(strFile,strVer);

	return strVer;
}

BOOL FileOpHelper::GetPEFileVer(const CString& strFile, CString& strVer)
{	
	DWORD dwHight = 0;
	DWORD dwLow = 0;

	if (!GetPEFileVer(strFile, dwHight, dwLow))
		return FALSE;

	strVer.Format(_T("%d.%d.%d.%04d"), HIWORD(dwHight), LOWORD(dwHight), HIWORD(dwLow), LOWORD(dwLow));

	return TRUE;
}

BOOL FileOpHelper::SafeWriteToFile(LPCTSTR pcsDstFile, BYTE* bySrc, DWORD dwSrcLen)
{
	if(NULL == pcsDstFile || NULL == bySrc || dwSrcLen <= 0)
		return FALSE;

	BOOL bSuccess = FALSE;
	HANDLE hDst = INVALID_HANDLE_VALUE;
	CString strDstFile = pcsDstFile;
	CString strDstFileBak = strDstFile + _T(".entbak");

	do
	{
		//如果目标文件所在的文件夹都不存在，先创建一下，不然createfile会失败
		int nPos = strDstFile.ReverseFind(L'\\');
		if(-1 != nPos)
		{
			CString strDstDir = strDstFile.Left(nPos+1);
			if(!PathFileExists(strDstDir))
				::CreateDirectory(strDstDir,NULL);
		}

		//目标文件已经存在，则先保存一个备份
		if(::PathFileExists(pcsDstFile))
			MoveFileEx(strDstFile,strDstFileBak,MOVEFILE_REPLACE_EXISTING);

		// 经鉴定，CopyFile的实现如果目标被只读共享方式打开，它必然失败，如果目标被共享读写方式打开，则它可以成功
		hDst = ::CreateFile(pcsDstFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hDst == INVALID_HANDLE_VALUE) 
			break;

		// 1. 截断目标文件
		BOOL bRet = ::SetEndOfFile(hDst);
		if(!bRet) 
			break;

		DWORD dwCopySize = dwSrcLen;
		for( ; ; )
		{
			DWORD dwWrited = 0;
			DWORD dwHave = (dwSrcLen > 16 * 1024) ? 16 * 1024 : dwSrcLen;

			bRet = ::WriteFile(hDst, bySrc, dwHave, &dwWrited, NULL);
			if(!bRet || dwHave != dwWrited)
				break;

			bySrc += dwHave;
			dwSrcLen -= dwHave;

			if(dwSrcLen <= 0|| !bRet)
				break;
		}
		::FlushFileBuffers(hDst);

		DWORD dwNewSize = GetFileSize(hDst, NULL);
		if(dwNewSize == dwCopySize)
			bSuccess = TRUE;

	} while (FALSE);

	if(hDst != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hDst);
		hDst = INVALID_HANDLE_VALUE;
	}

	if(!bSuccess)
	{
		::DeleteFile(pcsDstFile);
		if(::PathFileExists(strDstFileBak))
			MoveFileEx(strDstFileBak,strDstFile,MOVEFILE_REPLACE_EXISTING);
	}
	else
		DeleteFile(strDstFileBak);

	return bSuccess;
}

BOOL FileOpHelper::GetFileFromRes(UINT nResID, LPCTSTR lpcszFile)
{
	int nResult = FALSE;
	int nRetCode = FALSE;
	DWORD dwFileSize = 0;

	HRSRC hRc = NULL;
	HGLOBAL hGobal = NULL;
	BYTE* pbyDataExe = NULL;

	do 
	{
		hRc = ::FindResource(NULL, MAKEINTRESOURCE(nResID), _T("BIN"));
		if (NULL == hRc)
			break;

		dwFileSize = ::SizeofResource(NULL, hRc);
		if (dwFileSize <= 0)
			break;

		hGobal = ::LoadResource(NULL, hRc);
		if (NULL == hGobal)
			break;

		pbyDataExe = (LPBYTE)::LockResource(hGobal);
		if (NULL == pbyDataExe)
			break;

		nRetCode = SafeWriteToFile(lpcszFile, pbyDataExe, dwFileSize);
		if (!nRetCode)
			break;

		nResult = TRUE;

	} while (FALSE);


	if (NULL != hGobal)
	{
		if (NULL != pbyDataExe)
		{
			UnlockResource(hGobal);
			pbyDataExe = NULL;
		}

		::FreeResource(hGobal);
		hGobal = NULL;
	}

	hRc = NULL;

	return nResult;
}

BOOL FileOpHelper::GetFileFromRes(LPCTSTR lpSrcFile,LPCTSTR lpResType,int nResID, LPCTSTR lpDstFile)
{
	if(!lpSrcFile || !lpResType || !lpDstFile || !PathFileExists(lpSrcFile))
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;
	DWORD dwFileSize = 0;

	HRSRC hRc = NULL;
	HGLOBAL hGobal = NULL;
	BYTE* pbyDataExe = NULL;
	HMODULE hSrc = NULL;

	do 
	{
		hSrc = LoadLibrary(lpSrcFile);
		if(!hSrc)
			break;

		hRc = ::FindResource(hSrc, MAKEINTRESOURCE(nResID), lpResType);
		if (NULL == hRc)
			break;

		dwFileSize = ::SizeofResource(hSrc, hRc);
		if (dwFileSize <= 0)
			break;

		hGobal = ::LoadResource(hSrc, hRc);
		if (NULL == hGobal)
			break;

		pbyDataExe = (LPBYTE)::LockResource(hGobal);
		if (NULL == pbyDataExe)
			break;

		nRetCode = SafeWriteToFile(lpDstFile, pbyDataExe, dwFileSize);
		if (!nRetCode)
			break;

		nResult = TRUE;

	} while (FALSE);


	if (NULL != hGobal)
	{
		if (NULL != pbyDataExe)
		{
			UnlockResource(hGobal);
			pbyDataExe = NULL;
		}

		::FreeResource(hGobal);
		hGobal = NULL;
	}

	hRc = NULL;
	return nResult;
}

BOOL FileOpHelper::SafeDeleteFile(LPCTSTR lpFile)
{
	if(!::PathFileExists(lpFile))
		return TRUE;

	DWORD dwRet = 0;
	DWORD dwLen = MAX_PATH;
	TCHAR szTempPath[MAX_PATH] = { 0 };
	dwRet = ::GetTempPath(dwLen, szTempPath);
	if (dwRet <= 0 || dwRet > dwLen)
		return FALSE;

	TCHAR szTempFile[MAX_PATH] = { 0 };
	dwRet = ::GetTempFileName(szTempPath, 0, 0, szTempFile);
	if (_tcslen(szTempFile) > 0 && ::PathFileExists(lpFile))
	{
		::MoveFileEx(lpFile,szTempFile,MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
		::DeleteFile(szTempFile);
	}

	return TRUE;
}

int FileOpHelper::ReplaceFile(LPCTSTR lpSrcFile,LPCTSTR lpDstFile,BOOL bCheckVer /* = FALSE */,BOOL bCheckComments /* = FALSE */,LPCTSTR lpComments /* = L */,
							   LPCTSTR lpBackupName /* = L */)
{
	if(!::PathFileExists(lpSrcFile) || !SmartHasValidMySign(lpSrcFile))
	{
		WRITE_LOG(L"%s校验签名失败",lpSrcFile);
		return FALSE;
	}

	//目标文件不存在，直接拷贝
	if (!::PathFileExists(lpDstFile))
		return SafeCopyFile(lpSrcFile, lpDstFile);

	BYTE pmd5Src[64] = { 0 };
	BYTE pmd5Dst[64] = { 0 };
	CodecHelper::EntMd5File(lpSrcFile, pmd5Src);
	CodecHelper::EntMd5File(lpDstFile, pmd5Dst);
	if (memcmp(pmd5Src, pmd5Dst, 64) == 0)
	{
		WRITE_LOG(L"%s与%smd5一致",lpSrcFile,lpDstFile);
		return TRUE;
	}

	if(bCheckVer)
	{
		DWORD dwLowSrc = 0,dwHightSrc = 0,dwLowDst = 0,dwHighDst = 0;
		if ( GetPEFileVer(lpSrcFile, dwHightSrc, dwLowSrc) && GetPEFileVer(lpDstFile, dwHighDst, dwLowDst))
		{
			if( dwHightSrc < dwHighDst || (dwHightSrc == dwHighDst && dwLowSrc <= dwLowDst))
				return TRUE;
		}
	}

	//没有Comments信息或不等于指定值，则认为不是小助手已经作过替换的文件，需要先保存这个文件为lpBackupName
	if(bCheckComments && _tcslen(lpBackupName))
	{
		int nRetCode = FALSE;
		CString strVerComment;
		nRetCode = GetFileVersionCommentInfo(lpDstFile, strVerComment);
		if (!nRetCode || strVerComment.IsEmpty() || strVerComment.CompareNoCase(lpComments) != 0)
		{
			nRetCode = SafeCopyFile(lpDstFile, lpBackupName);
			if (!nRetCode)
			{
				WRITE_LOG(L"%s移动到%s，返回%d",lpDstFile,lpBackupName,nRetCode);
				return nRetCode;
			}
		}	
	}

	return SafeCopyFile(lpSrcFile,lpDstFile);
}

int FileOpHelper::ReplaceFileByRes(DWORD dwResID,LPCTSTR lpSrcMD5,DWORD dwResVerHigh,DWORD dwResVerLow,LPCTSTR lpDstFile,BOOL bCheckVer /* = FALSE */,BOOL bCheckComments /* = FALSE */,
									LPCTSTR lpComments /* = L */,LPCTSTR lpBackupName /* = L */)
{
	if(!::PathFileExists(lpDstFile) || !SmartHasValidMySign(lpDstFile))
	{
		SafeDeleteFile(lpDstFile);
		if(!GetFileFromRes(dwResID, lpDstFile) || !SmartHasValidMySign(lpDstFile)) //释放后的文件也要检查签名，不正确则删除文件
		{
			::DeleteFile(lpDstFile);
			return FALSE;
		}
		return TRUE;
	}

	if (_tcslen(lpSrcMD5))
	{
		CString strFileMD5;
		GetFileMD5String(lpDstFile, strFileMD5, 16);
		if (strFileMD5.CompareNoCase(lpSrcMD5) == 0)
			return TRUE;
	}

	if (bCheckVer)
	{
		DWORD dwDstVerLow = 0,dwDstVerHight = 0;
		if (GetPEFileVer(lpDstFile, dwDstVerHight, dwDstVerLow))
		{
			if(dwResVerHigh < dwDstVerHight || (dwResVerHigh == dwDstVerHight && dwResVerLow <= dwDstVerLow))
				return FALSE;
		}
	}

	//没有Comments信息或不等于指定值，则认为不是小助手已经作过替换的文件，需要先保存这个文件为lpBackupName
	if(bCheckComments && _tcslen(lpBackupName))
	{
		int nRetCode = FALSE;
		CString strVerComment;
		nRetCode = GetFileVersionCommentInfo(lpDstFile, strVerComment);
		if (!nRetCode || strVerComment.IsEmpty() || strVerComment.CompareNoCase(lpComments) != 0)
		{
			nRetCode = SafeCopyFile(lpDstFile, lpBackupName);
			if (!nRetCode)
				return nRetCode;
		}	
	}

	if(!GetFileFromRes(dwResID, lpDstFile) || !SmartHasValidMySign(lpDstFile)) //释放后的文件也要检查签名，不正确则删除文件
	{
		::DeleteFile(lpDstFile);
		return FALSE;
	}
	return TRUE;
}

BOOL FileOpHelper::SafeCopyFile(LPCTSTR src, LPCTSTR dst)
{
	BOOL success = FALSE;
	HANDLE hSrc = INVALID_HANDLE_VALUE;
	HANDLE hDst = INVALID_HANDLE_VALUE;

	CString strDstFile = dst;
	CString strDstFileBak = strDstFile + _T(".entbak");
	do
	{
		//如果目标文件所在的文件夹都不存在，先创建一下，不然CreateFile会失败
		int nPos = strDstFile.ReverseFind(L'\\');
		if(-1 != nPos)
		{
			CString strDstDir = strDstFile.Left(nPos+1);
			if(!PathFileExists(strDstDir))
				::CreateDirectory(strDstDir,NULL);
		}

		//目标文件已经存在，则先保存一个备份
		if(::PathFileExists(strDstFile))
			MoveFileEx(strDstFile,strDstFileBak,MOVEFILE_REPLACE_EXISTING);

		// 经鉴定，CopyFile的实现如果目标被只读共享方式打开，它必然失败
		// 如果目标被共享读写方式打开，则它可以成功
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

		// 1. 截断目标文件
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

BOOL FileOpHelper::DeleteDir(LPCTSTR lpDirPath)
{
	if(!lpDirPath || !::PathFileExists(lpDirPath))
		return FALSE;

	CString strFiles = lpDirPath;
	strFiles += L"\\*.*";

	WIN32_FIND_DATA wfd;
	HANDLE hFile = ::FindFirstFile(strFiles, &wfd);
	if( INVALID_HANDLE_VALUE == hFile)
		return FALSE;

	BOOL bSuccess = TRUE;
	while(bSuccess)
	{
		bSuccess = ::FindNextFile(hFile,&wfd);

		if(_tcscmp(wfd.cFileName,_T(".")) == 0 || _tcscmp(wfd.cFileName, _T("..")) == 0)
			continue;
		
		CString strFile = lpDirPath;
		strFile += L"\\";
		strFile += wfd.cFileName;
		::DeleteFile(strFile);
	}
	::FindClose(hFile);

	return TRUE;
}

BOOL FileOpHelper::IsPEFile(PCTSTR pFileName, BOOL& bPE)
{	
	bPE = FALSE;

	HANDLE hFile = CreateFile(pFileName, 
		GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
		NULL, 
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;


	IMAGE_DOS_HEADER dosHeader = { 0 };
	DWORD dwBytesRead = 0;

	ReadFile(hFile, 
		&dosHeader, 
		sizeof(dosHeader),
		&dwBytesRead,
		NULL);

	CloseHandle(hFile);

	if ((dwBytesRead == sizeof(dosHeader)) && 
		(dosHeader.e_magic == IMAGE_DOS_SIGNATURE))
	{
		bPE = TRUE;
	}

	return TRUE;
}

BOOL FileOpHelper::GetSignName(CString& strFile, CString& strSignName)
{
	if (strFile.IsEmpty())
	{
		return FALSE;
	}
	BOOL fResult;
	DWORD dwEncoding, dwContentType, dwFormatType;
	HCERTSTORE hStore = NULL;
	HCRYPTMSG hMsg = NULL;
	//查询签名信息
	fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
		(LPVOID)strFile.GetBuffer(),
		CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
		CERT_QUERY_FORMAT_FLAG_BINARY,
		0,
		&dwEncoding,
		&dwContentType,
		&dwFormatType,
		&hStore,
		&hMsg,
		NULL);
	if (!fResult)
	{
		WRITE_LOG(_T("CryptQueryObject failed with %x\n"), GetLastError());
		return FALSE;
	}
	// Get signer information size.
	DWORD dwSignerInfo;
	fResult = CryptMsgGetParam(hMsg,
		CMSG_SIGNER_INFO_PARAM,
		0,
		NULL,
		&dwSignerInfo);
	if (!fResult)
	{
		WRITE_LOG(_T("CryptMsgGetParam failed with %x\n"), GetLastError());
		return FALSE;
	}
	// Allocate memory for signer information.
	PCMSG_SIGNER_INFO pSignerInfo = NULL;
	pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
	if (!pSignerInfo)
	{
		WRITE_LOG(_T("Unable to allocate memory for Signer Info.\n"));
		return FALSE;
	}
	// Get Signer Information.
	fResult = CryptMsgGetParam(hMsg,
		CMSG_SIGNER_INFO_PARAM,
		0,
		(PVOID)pSignerInfo,
		&dwSignerInfo);
	if (!fResult)
	{
		WRITE_LOG(_T("CryptMsgGetParam failed with %x\n"), GetLastError());
		return FALSE;
	}

	// Search for the signer certificate in the temporary
	// certificate store.
	CERT_INFO CertInfo;
	CertInfo.Issuer = pSignerInfo->Issuer;
	CertInfo.SerialNumber = pSignerInfo->SerialNumber;
	PCCERT_CONTEXT pCertContext = CertFindCertificateInStore(hStore,
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		0,
		CERT_FIND_SUBJECT_CERT,
		(PVOID)&CertInfo,
		NULL);
	if (!pCertContext)
	{
		WRITE_LOG(_T("CertFindCertificateInStore failed with %x\n"),
			GetLastError());
		return FALSE;
	}
	LPTSTR szName = NULL;
	DWORD dwData;
	if (!(dwData = CertGetNameString(pCertContext,
		CERT_NAME_SIMPLE_DISPLAY_TYPE,
		0,
		NULL,
		NULL,
		0)))
	{
		WRITE_LOG(_T("CertGetNameString failed.\n"));
		return FALSE;
	}
	// Allocate memory for subject name.
	szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
	if (!szName)
	{
		WRITE_LOG(_T("Unable to allocate memory for subject name.\n"));
		return FALSE;
	}
	// Get subject name.
	if (!(CertGetNameString(pCertContext,
		CERT_NAME_SIMPLE_DISPLAY_TYPE,
		0,
		NULL,
		szName,
		dwData)))
	{
		WRITE_LOG(_T("CertGetNameString failed.\n"));
		return FALSE;
	}
	strSignName = szName;
	return TRUE;
}

static void SplitString(CString strLine,std::vector<CString>& vec_rlt,const CString& strSep /* = T("|")*/)
{
	vec_rlt.clear();
	strLine.Trim();
	while(!strLine.IsEmpty())
	{
		int nPos = strLine.Find(strSep);
		if(-1 != nPos)
		{
			vec_rlt.push_back(strLine.Left(nPos));
			strLine = strLine.Mid(nPos + strSep.GetLength());
		}
		else
		{
			vec_rlt.push_back(strLine);
			strLine = _T("");
		}
	}
}

int FileOpHelper::CompareVersion(CString ver1,CString ver2,const CString& strSep1 /* = L"."*/,const CString& strSep2 /* = L"."*/)
{
	std::vector<CString> vec_str1,vec_str2;
	SplitString(ver1,vec_str1,strSep1);
	SplitString(ver2,vec_str2,strSep2);
	size_t nLen = vec_str1.size();
	if(nLen > vec_str2.size())
		nLen = vec_str2.size();

	for(size_t i = 0; i < nLen; i++)
	{
		int num1 = _ttoi(vec_str1[i].GetBuffer());
		int num2 = _ttoi(vec_str2[i].GetBuffer());
		if(num1 != num2)
			return num1 - num2;
	}

	return vec_str1.size() - vec_str2.size();
}
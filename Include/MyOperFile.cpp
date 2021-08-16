#pragma once
#include "StdAfx.h"
#include "MyOperfile.h"

enum enumCodingMode
{
	enumCodingModeANSI = 1,
	enumCodingModeUTF8 = 2,
	enumCodingModeUnicode = 3,
};

BOOL MyOperFile::FileIsEixst( IN const std::wstring& strFilePath )
{
	if( (_waccess( strFilePath.c_str(), 0 )) != -1 )
		return TRUE;
	else
		return FALSE;
}

int MyOperFile::GetFileSize( IN const std::wstring& strFilePath, OUT size_t& uFileSize )
{

	size_t size = INVALID_FILE_SIZE;
	HANDLE	hFile = ::CreateFileW(
		strFilePath.c_str(),
		FILE_READ_ATTRIBUTES,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		size = ::GetFileSize(hFile, NULL);
		::CloseHandle(hFile);
		if (size == INVALID_FILE_SIZE && GetLastError() != NO_ERROR)
			return FALSE;

		uFileSize = size;

		return TRUE;
	}

	return FALSE;
}


int MyOperFile::ReadFileData( IN const std::wstring& strFilePath, OUT int& nMode, OUT std::wstring& strFileData )
{
	strFileData.clear();
	nMode = enumCodingModeUTF8;

	HANDLE  hFile = ::CreateFileW(strFilePath.c_str(), 
		FILE_READ_DATA, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwFileSize = ::GetFileSize(hFile, NULL);

	BYTE* pszData = new BYTE[dwFileSize + 2];
	BYTE* pNewData = NULL;
	ZeroMemory(pszData, dwFileSize + 2);
	DWORD nByeRead = 0;
	if (!::ReadFile(hFile, pszData, dwFileSize, &nByeRead, NULL))
	{
		if (pszData != NULL)
		{
			delete[] pszData;
			pszData = NULL;
		}
		CloseHandle(hFile);
		return FALSE;
	}

	if (pszData[0] == 0xFF && pszData[1] == 0xFE)
	{
		nMode = enumCodingModeUnicode;
		//pszData += 2;
		pNewData = pszData + 2;

		strFileData = (wchar_t *)pNewData;
	}
	else if (pszData[0] == 0xEF && pszData[1] == 0xBB && pszData[2] == 0xBF)
	{
		nMode = enumCodingModeUTF8;
		//pszData += 3;
		pNewData = pszData + 3;

		int nLen = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pNewData, -1, NULL, 0);  
		if (nLen == 0)	return FALSE; 

		wchar_t* pTempData = new wchar_t[nLen + 1];  
		ZeroMemory(pTempData, (nLen + 1) * 2);
		::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pNewData, -1, pTempData, nLen);  
		strFileData = pTempData;

		if (pTempData != NULL)
		{
			delete[] pTempData;
			pTempData = NULL;
		}

	}
	else
	{
		nMode = enumCodingModeUTF8;
		//pszData += 3;
		pNewData = pszData;// + 3;

		int nLen = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pNewData, -1, NULL, 0);  
		if (nLen == 0)	return FALSE; 

		wchar_t* pTempData = new wchar_t[nLen + 1];  
		ZeroMemory(pTempData, (nLen + 1) * 2);
		::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pNewData, -1, pTempData, nLen);  
		strFileData = pTempData;

		if (pTempData != NULL)
		{
			delete[] pTempData;
			pTempData = NULL;
		}
	}

	if (pszData != NULL)
	{
		delete[] pszData;
		pszData = NULL;
	}
	CloseHandle(hFile);

	return TRUE;
}

int MyOperFile::ReadFileData( IN const std::wstring& strFilePath, IN BYTE* pData, int nSize, int& nFileSize )
{

	HANDLE  hFile = ::CreateFileW(strFilePath.c_str(), 
		FILE_READ_DATA, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	nFileSize = ::GetFileSize(hFile, NULL);
	if (nFileSize > nSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD nByeRead = 0;
	if (!::ReadFile(hFile, pData, nFileSize, &nByeRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	return TRUE;
}


int MyOperFile::WriteFileData( IN const std::wstring& strFilePath, IN int nMode, IN std::wstring strFileData )
{
	//open -> create
	HANDLE  hFile = ::CreateFileW(strFilePath.c_str(), 
		FILE_WRITE_DATA, 
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	std::string strToFileData;
	if (nMode == enumCodingModeUTF8)
	{

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, strFileData.c_str(), -1, NULL, 0, NULL, NULL);  
		if (nLen == 0) return FALSE;  

		char* pUtf8Temp = new char[nLen + 1];
		ZeroMemory(pUtf8Temp, nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, strFileData.c_str(), -1, pUtf8Temp, nLen, NULL, NULL);  

		char szHead[3] = { '\xEF', '\xBB', '\xBF'};
		DWORD nByeRead = 0;
		BOOL bRet = ::WriteFile(hFile, szHead, 3, &nByeRead, NULL);
		if (!bRet)
		{
			if (pUtf8Temp != NULL)
			{
				delete[] pUtf8Temp;
				pUtf8Temp = NULL;
			}

			CloseHandle(hFile);
			return FALSE;
		}

		bRet = ::WriteFile(hFile, pUtf8Temp, nLen-1, &nByeRead, NULL);
		if (!bRet)
		{
			if (pUtf8Temp != NULL)
			{
				delete[] pUtf8Temp;
				pUtf8Temp = NULL;
			}

			CloseHandle(hFile);
			return FALSE;
		}

		if (pUtf8Temp != NULL)
		{
			delete[] pUtf8Temp;
			pUtf8Temp = NULL;
		}

		CloseHandle(hFile);
		return bRet;
	}
	else if (nMode == enumCodingModeUnicode)
	{
		DWORD nByeRead = 0;
		char szHead[2] = { '\xff', '\xfe'};
		BOOL bRet = ::WriteFile(hFile, szHead, 2, &nByeRead, NULL);
		if (!bRet)
		{
			CloseHandle(hFile);
			return FALSE;
		}

		bRet = ::WriteFile(hFile, strFileData.c_str(), strFileData.size() * 2, &nByeRead, NULL);
		if (!bRet)
		{
			CloseHandle(hFile);
			return FALSE;
		}

		CloseHandle(hFile);
		return TRUE;	
	}
	else if (nMode == enumCodingModeANSI)
	{
		DWORD nByeRead = 0;
		char* cpFileDataT = CW2A(strFileData.c_str());	//此处有可能有问题
		std::string strFileDataT = cpFileDataT;
		BOOL bRet = ::WriteFile(hFile, strFileDataT.c_str(), strFileDataT.size(), &nByeRead, NULL);
		if (!bRet)
		{
			CloseHandle(hFile);
			return FALSE;
		}

		CloseHandle(hFile);
		return TRUE;	
	}

	CloseHandle(hFile);
	return FALSE;

}

int MyOperFile::WriteFileData( IN const std::wstring& strFilePath, IN BYTE* pData, int nSize )
{
	HANDLE  hFile = ::CreateFileW(strFilePath.c_str(), 
		FILE_WRITE_DATA, 
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD nByeRead = 0;
	BOOL bRet = ::WriteFile(hFile, pData, nSize, &nByeRead, NULL);
	
	CloseHandle(hFile);
	return bRet;
}

int MyOperFile::DelteFile( IN const std::wstring& strFilePath, IN BOOL bRebootMoveIfFail /*= TRUE*/ )
{
	if (!FileIsEixst(strFilePath))
		return FALSE;

	int nRet = ::DeleteFileW(strFilePath.c_str());

	if (!nRet && bRebootMoveIfFail)
		nRet = ::MoveFileExW(strFilePath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

	return nRet;
}

int MyOperFile::FileCopy( IN const std::wstring& strSrcFilePath, IN const std::wstring& strDesFilePath, IN BOOL bFailIfExists )
{
	if (!FileIsEixst(strSrcFilePath))
		return FALSE;

	if (bFailIfExists)
		return ::CopyFileW(strSrcFilePath.c_str(), strDesFilePath.c_str(), true);
	else
		return ::CopyFileW(strSrcFilePath.c_str(), strDesFilePath.c_str(), false);
}
#include "StdAfx.h"
#include "XORFileEncrypt.h"

XORFileEncrypt::XORFileEncrypt(void)
{
	m_byKey = 0x88;
}

XORFileEncrypt::~XORFileEncrypt(void)
{
}

int XORFileEncrypt::XOREncrypt(LPBYTE pbyContent, DWORD dwLen, BYTE byKey)
{
	if (dwLen <= 0 || pbyContent == NULL)
		return FALSE;
	
	//if (IsBadWritePtr((LPVOID)pbyContent, nLen))
	//	return FALSE;

	//int nDword = nLen / 4;
	//int nBytes = nLen % 4;
	//DWORD dwKey = 0;

	//if (nDword > 0)
	//{
	//	dwKey = MAKELONG(MAKEWORD(byKey, byKey), MAKEWORD(byKey, byKey));
	//}

	for (size_t nIndex = 0; nIndex < dwLen; nIndex++)
	{
		pbyContent[nIndex] = pbyContent[nIndex] ^ byKey;
	}
	return TRUE;
}


int XORFileEncrypt::Encrypt(LPCTSTR lpcszFile)
{
	if (NULL == lpcszFile || !PathFileExists(lpcszFile))
		return FALSE;

	HANDLE hFile = ::CreateFile(lpcszFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return FALSE;

	DWORD dwHight = 0;
	DWORD dwFileSize = ::GetFileSize(hFile, &dwHight);
	if (dwHight > 0)
	{
		::CloseHandle(hFile);
		return FALSE;
	}

	if (dwFileSize <= 0)
	{
		::CloseHandle(hFile);
		return TRUE;
	}

	DWORD dwLen = dwFileSize + 32 + (32 - dwFileSize % 32);
	BYTE* byBuf = new BYTE[dwLen];
	if (NULL == byBuf)
	{
		::CloseHandle(hFile);
		return FALSE;
	}

	::ZeroMemory(byBuf, dwLen);
	
	DWORD dwRet = 0;
	if (!::ReadFile(hFile, byBuf, dwFileSize, &dwRet, NULL) || dwRet != dwFileSize)
	{
		delete[] byBuf;
		::CloseHandle(hFile);
		return FALSE;
	}

	if (!XOREncrypt(byBuf, dwFileSize, m_byKey))
	{
		delete[] byBuf;
		::CloseHandle(hFile);
		return FALSE;
	}

	LARGE_INTEGER lpos = { 0 };
	if (!::SetFilePointerEx(hFile, lpos, NULL, FILE_BEGIN))
	{
		delete[] byBuf;
		::CloseHandle(hFile);
		return FALSE;
	}

	if (!WriteFile(hFile, byBuf, dwFileSize, &dwRet, NULL) && dwRet != dwFileSize)
	{
		delete[] byBuf;
		::CloseHandle(hFile);
		return FALSE;
	}
	 
	::CloseHandle(hFile);
	delete[] byBuf;
    return TRUE;
}

int XORFileEncrypt::Decrypt(LPCTSTR lpcszFile)
{
	return Encrypt(lpcszFile);
}

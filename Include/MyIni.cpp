#pragma once



//#include "stdafx.h" // include if you got "fatal error C1010: unexpected end of file..."
#include <MyIni.h>
//#include <string.h>
//#include <stdio.h>
//#include <assert.h>


#define DEF_PROFILE_NUM_LEN		64 // 数字字符串长度，对于二进制格式可能很长
#define DEF_PROFILE_THRESHOLD	512 // 临时字符串长度
#define DEF_PROFILE_DELIMITER	_T(",") // 默认字符串分隔符
#define DEF_PROFILE_TESTSTRING	_T("{63788286-AE30-4D6B-95DF-3B451C1C79F9}") // 内部使用的 Uuid

// 这个结构体作为LPVOID参数用来给__KeyPairProc传参
struct STR_LIMIT
{
	LPTSTR lpTarget;
	DWORD dwRemain;
	DWORD dwTotalCopied;
};

/////////////////////////////////////////////////////////////////////////////////
// 构造函数 & 析构函数
/////////////////////////////////////////////////////////////////////////////////
IniManager::IniManager()
{
	m_pszPathName = NULL;
}

IniManager::IniManager(LPCTSTR lpPathName)
{
	m_pszPathName = NULL;
	SetPathName(lpPathName);
}

IniManager::~IniManager()
{
	if (m_pszPathName != NULL)
	{
		free(m_pszPathName);
		m_pszPathName = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Ini 文件路径访问
/////////////////////////////////////////////////////////////////////////////////


void IniManager::SetPathName(LPCTSTR lpPathName)
{
	if (lpPathName == NULL)
	{
		if (m_pszPathName != NULL)
			*m_pszPathName = _T('\0');
	}
	else
	{
		if (m_pszPathName != NULL)
		{
			free(m_pszPathName);
			m_pszPathName = NULL;
		}

		m_pszPathName = _tcsdup(lpPathName);
	}
}

CString IniManager::GetPathName() const
{
	return CString(m_pszPathName);
}


/////////////////////////////////////////////////////////////////////////////////
// 原始字符串访问
/////////////////////////////////////////////////////////////////////////////////

DWORD IniManager::GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	LPTSTR psz = __GetStringDynamic(lpSection, lpKey, lpDefault);
	DWORD dwLen = _tcslen(psz);

	if (lpBuffer != NULL)
	{
		_tcsncpy(lpBuffer, psz, dwBufSize);
		dwLen = min(dwLen, dwBufSize);
	}

	delete[] psz;
	return dwLen;
}

CString IniManager::GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault) const
{
	LPTSTR psz = __GetStringDynamic(lpSection, lpKey, lpDefault);
	CString str(psz);		
	delete [] psz;
	return str;
}



BOOL IniManager::WriteString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue) const
{
	if (lpSection == NULL || lpKey == NULL)
		return FALSE;

	return ::WritePrivateProfileString(lpSection, lpKey, lpValue == NULL ? _T("") : lpValue, m_pszPathName);
}


BOOL IniManager::AppendString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpString) const
{
	if (lpString == NULL)
		return FALSE;

	TCHAR* psz = __GetStringDynamic(lpSection, lpKey);
	TCHAR* pNewString = new TCHAR[_tcslen(psz) + _tcslen(lpString) + 1];
	_stprintf(pNewString, _T("%s%s"), psz, lpString);
	const BOOL RES = WriteString(lpSection, lpKey, pNewString);
	delete [] pNewString;
	delete [] psz;
	return RES;
}

/////////////////////////////////////////////////////////////////////////////////
// Ini 文件字符串数组访问
/////////////////////////////////////////////////////////////////////////////////

// 获取字符串数组

//DWORD IniManager::GetArray(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDelimiter, BOOL bTrimString) const
//{
//	if (lpBuffer != NULL)
//		*lpBuffer = _T('\0');
//
//	if (lpSection == NULL || lpKey == NULL)
//		return 0;
//
//	LPTSTR psz = __GetStringDynamic(lpSection, lpKey);
//
//	DWORD dwCopied = 0;
//
//	if (*psz != _T('\0'))
//	{
//		if (lpBuffer == NULL)
//		{
//			// 只需计算所需的缓冲区大小
//			const DWORD MAX_LEN = _tcslen(psz) + 2;
//			LPTSTR p = new TCHAR[MAX_LEN + 1];
//			dwCopied = __StringSplit(psz, p, MAX_LEN, lpDelimiter, bTrimString);
//			delete[] p;
//		}
//		else
//		{
//			dwCopied = __StringSplit(psz, lpBuffer, dwBufSize, lpDelimiter, bTrimString);
//		}
//	}
//
//	delete[] psz;
//	return dwCopied;
//}

//void IniManager::GetArray(LPCTSTR lpSection, LPCTSTR lpKey, CStringArray *pArray, LPCTSTR lpDelimiter, BOOL bTrimString) const
//{
//	if (pArray != NULL)
//		pArray->RemoveAll();
//
//	const DWORD LEN = GetArray(lpSection, lpKey, NULL, 0, lpDelimiter);
//	if (LEN == 0)
//		return;
//
//	LPTSTR psz = new TCHAR[LEN + 3];
//	GetArray(lpSection, lpKey, psz, LEN + 2, lpDelimiter);
//	ParseDNTString(psz, __SubStrAdd, (LPVOID)pArray);
//	delete [] psz;
//}



//BOOL IniManager::WriteArray(LPCTSTR lpSection, LPCTSTR lpKey, const CStringArray *pArray, int nWriteCount, LPCTSTR lpDelimiter) const
//{
//	if (pArray == NULL)
//		return FALSE;
//
//	if (nWriteCount < 0)
//		nWriteCount = pArray->GetSize();
//	else
//		nWriteCount = min(nWriteCount, pArray->GetSize());
//
//	const ATL::CString DELIMITER = (lpDelimiter == NULL || *lpDelimiter == _T('\0')) ? _T(",") : lpDelimiter;
//	ATL::CString sLine;
//	for (int i = 0; i < nWriteCount; i++)
//	{
//		sLine += pArray->GetAt(i);
//		if (i != nWriteCount - 1)
//			sLine += DELIMITER;
//	}
//	return WriteString(lpSection, lpKey, sLine);
//}


/////////////////////////////////////////////////////////////////////////////////
// 原始数据类型访问
/////////////////////////////////////////////////////////////////////////////////


int IniManager::GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase) const
{
	TCHAR sz[DEF_PROFILE_NUM_LEN + 1] = _T("");
	GetString(lpSection, lpKey, sz, DEF_PROFILE_NUM_LEN);
	return *sz == _T('\0') ? nDefault : int(_tcstoul(sz, NULL, __ValidateBase(nBase)));
}


UINT IniManager::GetUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nDefault, int nBase) const
{
	TCHAR sz[DEF_PROFILE_NUM_LEN + 1] = _T("");
	GetString(lpSection, lpKey, sz, DEF_PROFILE_NUM_LEN);
	return *sz == _T('\0') ? nDefault : UINT(_tcstoul(sz, NULL, __ValidateBase(nBase)));
}


BOOL IniManager::GetBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bDefault) const
{
	TCHAR sz[DEF_PROFILE_NUM_LEN + 1] = _T("");
	GetString(lpSection, lpKey, sz, DEF_PROFILE_NUM_LEN);
	return StringToBool(sz, bDefault);
}


double IniManager::GetDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fDefault) const
{
	TCHAR sz[DEF_PROFILE_NUM_LEN + 1] = _T("");
	GetString(lpSection, lpKey, sz, DEF_PROFILE_NUM_LEN);
	return *sz == _T('\0') ? fDefault : _tcstod(sz, NULL);
}


BOOL IniManager::WriteInt(LPCTSTR lpSection, LPCTSTR lpKey, int nValue, int nBase) const
{
	TCHAR szValue[DEF_PROFILE_NUM_LEN + 1] = _T("");
	__IntToString(nValue, szValue, nBase);
	return WriteString(lpSection, lpKey, szValue);
}


BOOL IniManager::WriteUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nValue, int nBase) const
{
	TCHAR szValue[DEF_PROFILE_NUM_LEN + 1] = _T("");
	__UIntToString(nValue, szValue, nBase);
	return WriteString(lpSection, lpKey, szValue);
}


BOOL IniManager::WriteDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fValue, int nPrecision) const
{
	TCHAR szFmt[16] = _T("%f");

	if (nPrecision > 0)
		_stprintf(szFmt, _T("%%.%df"), nPrecision);

	TCHAR szValue[DEF_PROFILE_NUM_LEN + 1] = _T("");
	_stprintf(szValue, szFmt, fValue);
	return WriteString(lpSection, lpKey, szValue);
}


BOOL IniManager::IncreaseDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fIncrease, int nPrecision) const
{
	double f = GetDouble(lpSection, lpKey, 0.0);
	f += fIncrease;
	return WriteDouble(lpSection, lpKey, f, nPrecision);
}


BOOL IniManager::WriteBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bValue) const
{
	return WriteInt(lpSection, lpKey, bValue ? 1 : 0, BASE_DECIMAL);
}


BOOL IniManager::InvertBool(LPCTSTR lpSection, LPCTSTR lpKey) const
{
	return WriteBool(lpSection, lpKey, !GetBool(lpSection, lpKey, FALSE));
}


BOOL IniManager::IncreaseInt(LPCTSTR lpSection, LPCTSTR lpKey, int nIncrease, int nBase) const
{
	int nVal = GetInt(lpSection, lpKey, 0, nBase);
	nVal += nIncrease;
	return WriteInt(lpSection, lpKey, nVal, nBase);
}


BOOL IniManager::IncreaseUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nIncrease, int nBase) const
{
	UINT nVal = GetUInt(lpSection, lpKey, 0, nBase);
	nVal += nIncrease;
	return WriteUInt(lpSection, lpKey, nVal, nBase);
}

TCHAR IniManager::GetChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR cDefault) const
{
	TCHAR sz[2] = _T("");
	GetString(lpSection, lpKey, sz, 1);
	return *sz == _T('\0') ? cDefault : sz[0];
}

BOOL IniManager::WriteChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR c) const
{
	TCHAR sz[2] = { c, _T('\0') };
	return WriteString(lpSection, lpKey, sz);
}

/////////////////////////////////////////////////////////////////////////////////
// 用户定义的数据类型访问
/////////////////////////////////////////////////////////////////////////////////

// 从 ini 文件中获取原始数据块
//DWORD IniManager::GetDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPVOID lpBuffer, DWORD dwBufSize, DWORD dwOffset) const
//{
//	LPTSTR psz = __GetStringDynamic(lpSection, lpKey);
//	DWORD dwLen = _tcslen(psz) / 2;
//	if (dwLen <= dwOffset)
//	{
//		delete [] psz;
//		return 0;
//	}
//
//	// verify psz, must be all in hex format
//	for (int i = 0; psz[i] != _T('\0'); i++)
//	{
//		TCHAR c = psz[i];
//		if ((c >= _T('0') && c <= _T('9'))
//			|| (c >= _T('a') && c <= _T('f'))
//			|| (c >= _T('A') && c <= _T('F')))
//		{
//			// valid
//		}
//		else
//		{
//			delete [] psz;
//			return 0;
//		}
//	}
//
//	DWORD dwProcLen = 0;
//	LPBYTE lpb = (LPBYTE)lpBuffer;
//
//	if (lpb != NULL)
//	{
//		dwProcLen = min(dwLen - dwOffset, dwBufSize);
//		LPCTSTR p = &psz[dwOffset * 2];
//		for (DWORD i = 0; i < dwProcLen; i++)
//		{			
//			TCHAR sz[3] = _T("");
//			_tcsncpy(sz, p, 2);			
//			lpb[i] = BYTE(_tcstoul(sz, NULL, 16));
//			p = &p[2];
//		}			
//	}
//	else
//	{
//		dwProcLen = dwLen - dwOffset;
//	}
//	delete [] psz;
//	return dwProcLen;
//}

// 将原始数据块写入 ini 文件
//BOOL IniManager::WriteDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const
//{
//	const BYTE* lpb = (const BYTE*)lpData;
//	if (lpb == NULL)
//		return FALSE;
//
//	LPTSTR psz = new TCHAR[dwDataSize * 2 + 1];
//	for (DWORD i = 0, j = 0; i < dwDataSize; i++, j += 2)
//		_stprintf(&psz[j], _T("%02X"), lpb[i]);
//	const BOOL RES = WriteString(lpSection, lpKey, psz);
//	delete [] psz;
//	return RES;
//}

// 将原始数据块附加到 ini 文件中的指定键
//BOOL IniManager::AppendDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const
//{
//	const BYTE* lpb = (const BYTE*)lpData;
//	if (lpb == NULL)
//		return FALSE;
//
//	LPTSTR psz = new TCHAR[dwDataSize * 2 + 1];
//	for (DWORD i = 0, j = 0; i < dwDataSize; i++, j += 2)
//		_stprintf(&psz[j], _T("%02X"), lpb[i]);
//	const BOOL RES = AppendString(lpSection, lpKey, psz);
//	delete [] psz;
//	return RES;
//}

// 获取 POINT 值
//POINT IniManager::GetPoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT ptDefault) const
//{
//	POINT pt;
//	if (GetDataBlock(lpSection, lpKey, &pt, sizeof(POINT)) != sizeof(POINT))
//		pt = ptDefault;
//	return pt;
//}

// 获取 RECT 值
//RECT IniManager::GetRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rcDefault) const
//{
//	RECT rc;
//	if (GetDataBlock(lpSection, lpKey, &rc, sizeof(RECT)) != sizeof(RECT))
//		rc = rcDefault;
//	return rc;
//}

// 将 POINT 写入 ini 文件
//BOOL IniManager::WritePoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT pt) const
//{
//	return WriteDataBlock(lpSection, lpKey, &pt, sizeof(POINT));
//}

// 将 RECT 写入 ini 文件
//BOOL IniManager::WriteRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rc) const
//{
//	return WriteDataBlock(lpSection, lpKey, &rc, sizeof(RECT));
//}

/////////////////////////////////////////////////////////////////////////////////
// 部分和键访问
/////////////////////////////////////////////////////////////////////////////////

// 检索指定部分的密钥行（密钥对）列表
DWORD IniManager::GetKeyLines(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	if (lpSection == NULL)
		return 0;	

	if (lpBuffer == NULL)
	{
		// 只需计算所需的缓冲区大小
		DWORD dwLen = DEF_PROFILE_THRESHOLD;
		LPTSTR psz = new TCHAR[dwLen + 1];
		DWORD dwCopied = ::GetPrivateProfileSection(lpSection, psz, dwLen, m_pszPathName);

		while (dwCopied + 2 >= dwLen)
		{
			dwLen += DEF_PROFILE_THRESHOLD;
			delete [] psz;
			psz = new TCHAR[dwLen + 1];
			dwCopied = ::GetPrivateProfileSection(lpSection, psz, dwLen, m_pszPathName);
		}

		delete [] psz;
		return dwCopied + 2;
	}
	else
	{
		return ::GetPrivateProfileSection(lpSection, lpBuffer, dwBufSize, m_pszPathName);
	}
}

// 检索指定部分的键名列表
DWORD IniManager::GetKeyNames(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	if (lpSection == NULL)
		return 0;	

	STR_LIMIT sl;	
	sl.lpTarget = lpBuffer;
	sl.dwRemain = dwBufSize;
	sl.dwTotalCopied = 0;

	const DWORD LEN = GetKeyLines(lpSection, NULL, 0);
	if (LEN == 0)
		return 0;

	LPTSTR psz = new TCHAR[LEN + 1];
	GetKeyLines(lpSection, psz, LEN);
	ParseDNTString(psz, __KeyPairProc, (LPVOID)(&sl));
	delete [] psz;
	if (lpBuffer != NULL)
		lpBuffer[sl.dwTotalCopied] = _T('\0');
	return sl.dwTotalCopied;
}

// 从 ini 文件中获取所有部分名称
DWORD IniManager::GetSectionNames(LPTSTR lpBuffer, DWORD dwBufSize) const
{
	if (lpBuffer == NULL)
	{
		// 只需计算所需的缓冲区大小
		DWORD dwLen = DEF_PROFILE_THRESHOLD;
		LPTSTR psz = new TCHAR[dwLen + 1];
		DWORD dwCopied = ::GetPrivateProfileSectionNames(psz, dwLen, m_pszPathName);
		while (dwCopied + 2 >= dwLen)
		{
			dwLen += DEF_PROFILE_THRESHOLD;
			delete[] psz;
			psz = new TCHAR[dwLen + 1];
			dwCopied = ::GetPrivateProfileSectionNames(psz, dwLen, m_pszPathName);
		}

		delete[] psz;
		return dwCopied + 2;
	}
	else
	{
		return ::GetPrivateProfileSectionNames(lpBuffer, dwBufSize, m_pszPathName);
	}
}

//void IniManager::GetSectionNames(CStringArray *pArray) const
//{
//	if (pArray != NULL)
//		pArray->RemoveAll();
//
//	const DWORD LEN = GetSectionNames(NULL, 0);
//	if (LEN == 0)
//		return;
//
//	LPTSTR psz = new TCHAR[LEN + 1];
//	GetSectionNames(psz, LEN);
//	ParseDNTString(psz, __SubStrAdd, pArray);
//	delete [] psz;
//}



// 检索指定部分的密钥行（密钥对）列表
//void IniManager::GetKeyLines(LPCTSTR lpSection, CStringArray *pArray) const
//{
//	if (pArray != NULL)
//		pArray->RemoveAll();
//
//	const DWORD LEN = GetKeyLines(lpSection, NULL, 0);
//	if (LEN == 0)
//		return;
//
//	LPTSTR psz = new TCHAR[LEN + 1];
//	GetKeyLines(lpSection, psz, LEN);
//	ParseDNTString(psz, __SubStrAdd, pArray);
//	delete [] psz;
//}



// 检索指定部分的键名列表
//void IniManager::GetKeyNames(LPCTSTR lpSection, CStringArray *pArray) const
//{
//	if (pArray == NULL)
//		return;
//
//	pArray->RemoveAll();
//	const DWORD LEN = GetKeyNames(lpSection, NULL, 0);
//	LPTSTR psz = new TCHAR[LEN + 1];
//	GetKeyNames(lpSection, psz, LEN);
//	ParseDNTString(psz, __SubStrAdd, (LPVOID)pArray);
//	delete [] psz;
//}


// 从 ini 文件中删除整个部分
BOOL IniManager::DeleteSection(LPCTSTR lpSection) const
{
	return ::WritePrivateProfileString(lpSection, NULL, _T(""), m_pszPathName);
}

// 从部分中删除一个键
BOOL IniManager::DeleteKey(LPCTSTR lpSection, LPCTSTR lpKey) const
{
	return ::WritePrivateProfileString(lpSection, lpKey, NULL, m_pszPathName);
}

BOOL IniManager::IsSectionExist(LPCTSTR lpSection) const
{
	if (lpSection == NULL)
		return FALSE;

	// 首先获取段名列表，然后检查列表中是否存在lpSection。
	const DWORD LEN = GetSectionNames(NULL, 0);
	if (LEN == 0)
		return FALSE;

	LPTSTR psz = new TCHAR[LEN + 1];
	GetSectionNames(psz, LEN);
	BOOL RES = !ParseDNTString(psz, __SubStrCompare, (LPVOID)lpSection);
	delete [] psz;
	return RES;
}

BOOL IniManager::IsKeyExist(LPCTSTR lpSection, LPCTSTR lpKey) const
{
	if (lpSection == NULL || lpKey == NULL)
		return FALSE;

	// 使用默认的唯一字符串进行测试
	LPTSTR psz = __GetStringDynamic(lpSection, lpKey, DEF_PROFILE_TESTSTRING);
	const BOOL RES = (_tcscmp(psz, DEF_PROFILE_TESTSTRING) != 0);
	delete [] psz;
	return RES;
}

BOOL IniManager::CopySection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist) const
{
	if (lpSrcSection == NULL || lpDestSection == NULL)
		return FALSE;

	if (_tcsicmp(lpSrcSection, lpDestSection) == 0)
		return FALSE;

	if (!IsSectionExist(lpSrcSection))
		return FALSE;

	if (bFailIfExist && IsSectionExist(lpDestSection))
		return FALSE;

	DeleteSection(lpDestSection);

	const DWORD SRC_LEN = GetKeyLines(lpSrcSection, NULL, 0);
	LPTSTR psz = new TCHAR[SRC_LEN + 2];
	//memset(psz, 0, sizeof(TCHAR) * (SRC_LEN + 2));
	GetKeyLines(lpSrcSection, psz, SRC_LEN);	
	const BOOL RES = ::WritePrivateProfileSection(lpDestSection, psz, m_pszPathName);
	delete [] psz;

	return RES;
}

BOOL IniManager::CopyKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist) const
{
	if (lpSrcSection == NULL || lpSrcKey == NULL || lpDestKey == NULL)
		return FALSE;

	if (_tcsicmp(lpSrcSection, lpDestSection) == 0
		&& _tcsicmp(lpSrcKey, lpDestKey) == 0)
		return FALSE;

	if (!IsKeyExist(lpSrcSection, lpSrcKey))
		return FALSE;

	if (bFailIfExist && IsKeyExist(lpDestSection, lpDestKey))
		return FALSE;
	
	LPTSTR psz = __GetStringDynamic(lpSrcSection, lpSrcKey);
	const BOOL RES = WriteString(lpDestSection, lpDestKey, psz);
	delete [] psz;
	return RES;
}

BOOL IniManager::MoveSection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist) const
{
	return CopySection(lpSrcSection, lpDestSection, bFailIfExist)
		&& DeleteSection(lpSrcSection);
}

BOOL IniManager::MoveKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist) const
{
	return CopyKey(lpSrcSection, lpSrcKey, lpDestSection, lpDestKey, bFailIfExist)
		&& DeleteKey(lpSrcSection, lpSrcKey);
}

/////////////////////////////////////////////////////////////////////////////////
// 辅助函数
/////////////////////////////////////////////////////////////////////////////////

// 获取一个配置文件字符串值，返回一个堆指针所以我们不必担心
// 关于缓冲区大小，但是这个函数需要调用者手动释放内存。
// 该函数是该类所有“Getxxx”函数的主干。
LPTSTR IniManager::__GetStringDynamic(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault) const
{
	TCHAR* psz = NULL;
	if (lpSection == NULL || lpKey == NULL)
	{
		// 无效的部分或键名，只返回默认字符串
		if (lpDefault == NULL)
		{
			// 空字符串
			psz = new TCHAR[1];
			*psz = _T('\0');
		}
		else
		{
			psz = new TCHAR[_tcslen(lpDefault) + 1];
			_tcscpy(psz, lpDefault);
		}
		
		return psz;
	}
	
	// 不断扩大缓冲区大小，直到确定我们的字符串
	// 检索到的是原始的（未截断）。
	DWORD dwLen = DEF_PROFILE_THRESHOLD;
	psz = new TCHAR[dwLen + 1];
	DWORD dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == NULL ? _T("") : lpDefault, psz, dwLen, m_pszPathName);
	while (dwCopied + 1 >= dwLen)
	{		
		dwLen += DEF_PROFILE_THRESHOLD;
		delete [] psz;
		psz = new TCHAR[dwLen + 1];
		dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == NULL ? _T("") : lpDefault, psz, dwLen, m_pszPathName);
	}

	return psz; //！！！ 需要调用者释放这个内存！！！
}

// 使用特定的分隔符拆分字符串，拆分结果以“双空终止字符串”格式复制到lpBuffer中，如下图所示：xxx\0xxxx\0xx\0xxx\0\0
// 比如分隔符是","，那么字符串"ab,cd,e"会被拆分成"ab\0cd\0e\0\0"，这种字符串格式可以解析成子字符串数组 轻松使用用户定义的函数或 IniManager::ParseStringArray。
DWORD IniManager::__StringSplit(LPCTSTR lpString, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDelimiter, BOOL bTrimString)
{
	if (lpString == NULL || lpBuffer == NULL || dwBufSize == 0)
		return 0;	

	DWORD dwCopied = 0;
	*lpBuffer = _T('\0');
	if (*lpString == _T('\0'))
		return 0;

	// 如果 lpDelimiter 为 NULL，则使用默认的分隔符“,”，如果分隔符长度
	// 为 0，则返回整个字符串
	if (lpDelimiter != NULL && *lpDelimiter == _T('\0'))
	{
		_tcsncpy(lpBuffer, lpString, dwBufSize - 1);
		return _tcslen(lpBuffer);
	}

	LPTSTR pszDel = (lpDelimiter == NULL) ? _tcsdup(DEF_PROFILE_DELIMITER) : _tcsdup(lpDelimiter);
	const DWORD DEL_LEN = _tcslen(pszDel);
	LPTSTR lpTarget = lpBuffer;

	// 通过 lpString 搜索分隔符匹配，并提取出子字符串
	LPCTSTR lpPos = lpString;
	LPCTSTR lpEnd = _tcsstr(lpPos, pszDel);

	while (lpEnd != NULL)
	{
		LPTSTR pszSeg = __StrDupEx(lpPos, lpEnd);
		if (bTrimString)
			__TrimString(pszSeg);

		const DWORD SEG_LEN = _tcslen(pszSeg);
		const DWORD COPY_LEN = min(SEG_LEN, dwBufSize - dwCopied);

		// 需要避免缓冲区溢出
		if (COPY_LEN > 0)
		{
			dwCopied += COPY_LEN + 1;
			_tcsncpy(lpTarget, pszSeg, COPY_LEN);
			lpTarget[COPY_LEN] = _T('\0');
			lpTarget = &lpTarget[SEG_LEN + 1];
		}
		delete [] pszSeg;
		lpPos = &lpEnd[DEL_LEN]; // 前进指针进行下一次搜索
		lpEnd = _tcsstr(lpPos, pszDel);
	}

	// 字符串的最后一部分，可能没有尾部分隔符，所以我们
	// 也需要处理这部分
	LPTSTR pszSeg = _tcsdup(lpPos);
	if (bTrimString)
		__TrimString(pszSeg);

	const DWORD SEG_LEN = _tcslen(pszSeg);
	const DWORD COPY_LEN = min(SEG_LEN, dwBufSize - dwCopied);

	if (COPY_LEN > 0)
	{
		dwCopied += COPY_LEN + 1;
		_tcsncpy(lpTarget, pszSeg, COPY_LEN);
		lpTarget[COPY_LEN] = _T('\0');
	}

	delete [] pszSeg;
	lpBuffer[dwCopied] = _T('\0');
	delete [] pszDel;
	return dwCopied;
}

// 解析一个“双空终止字符串”，将每个子字符串传递给一个用户定义的
// 回调函数
BOOL IniManager::ParseDNTString(LPCTSTR lpString, SUBSTRPROC lpFnStrProc, LPVOID lpParam)
{
	if (lpString == NULL || lpFnStrProc == NULL)
		return FALSE;

	LPCTSTR p = lpString;
	DWORD dwLen = _tcslen(p);

	while (dwLen > 0)
	{
		if (!lpFnStrProc(p, lpParam))
			return FALSE;

		p = &p[dwLen + 1];
		dwLen = _tcslen(p);
	}
	return TRUE;
}

// 用于将“双空终止字符串”中的元素与给定字符串进行比较的回调函数。 用于在节名称列表中搜索。
BOOL CALLBACK IniManager::__SubStrCompare(LPCTSTR lpString1, LPVOID lpParam)
{
	assert(lpString1 != NULL);
	LPCTSTR lpString2 = (LPCTSTR)lpParam;
	assert(lpString2 != NULL);
	// 如果两个字符串匹配，则返回零以停止解析
	return _tcsicmp(lpString1, lpString2) != 0;
}

// 用于处理密钥对的回调函数，它从密钥对字符串中提取密钥名称
BOOL CALLBACK IniManager:: __KeyPairProc(LPCTSTR lpString, LPVOID lpParam)
{
	STR_LIMIT* psl = (STR_LIMIT*)lpParam;
	if (lpString == NULL || psl== NULL)
		return FALSE;
	
	LPCTSTR p = _tcschr(lpString, _T('='));
	if (p == NULL || p == lpString)
		return TRUE;
	
	// 提取 '=' 左侧的子字符串
	LPTSTR psz = new TCHAR[_tcslen(lpString) + 1];
	int i = 0;
	for (i = 0; &lpString[i] < p; i++)
		psz[i] = lpString[i];
	psz[i] = _T('\0');

	// trim
	__TrimString(psz);
	DWORD dwNameLen = _tcslen(psz);
	DWORD dwCopyLen = 0;
	
	//复制到缓冲区
	if (psl->lpTarget != NULL)
	{
		dwCopyLen = (psl->dwRemain > 1) ? min(dwNameLen, psl->dwRemain - 1) : 0;
		_tcsncpy(psl->lpTarget, psz, dwCopyLen);
		psl->lpTarget[dwCopyLen] = _T('\0');
		psl->lpTarget = &(psl->lpTarget[dwCopyLen + 1]); 
		psl->dwRemain -= dwCopyLen + 1;
	}
	else
	{
		dwCopyLen = dwNameLen;
	}

	delete [] psz;
	psl->dwTotalCopied += dwCopyLen + 1;
	return TRUE;
}

// 用于将从“双空终止字符串”中提取的元素添加到 MFC CStringArray 的回调函数。
//BOOL CALLBACK IniManager::__SubStrAdd(LPCTSTR lpString, LPVOID lpParam)
//{
//	CStringArray* pArray = (CStringArray*)lpParam;
//	if (pArray == NULL || lpString == NULL)
//		return FALSE;
//
//	pArray->Add(lpString);
//	return TRUE;
//}


// 将整数转换为二进制字符串格式
void IniManager::__ToBinaryString(UINT nNumber, LPTSTR lpBuffer, DWORD dwBufSize)
{
	if (dwBufSize == 0)
		return;
	
	DWORD dwIndex = 0;	
	do
	{
		lpBuffer[dwIndex++] = (nNumber % 2) ? _T('1') : _T('0');
		nNumber /= 2;
	} while (nNumber > 0 && dwIndex < dwBufSize);

	lpBuffer[dwIndex] = _T('\0');
	_tcsrev(lpBuffer);
}

// 确保基数为预期值
int IniManager::__ValidateBase(int nBase)
{
	switch (nBase)
	{
	case BASE_BINARY:
	case BASE_OCTAL:
	case BASE_HEXADECIMAL:
		break;

	default:
		nBase = BASE_DECIMAL;
	}

	return nBase;
}

// 根据其基数将有符号整数转换为字符串表示
void IniManager::__IntToString(int nNumber, LPTSTR lpBuffer, int nBase)
{
	switch (nBase)
	{
	case BASE_BINARY:
	case BASE_OCTAL:
	case BASE_HEXADECIMAL:
		__UIntToString((UINT)nNumber, lpBuffer, nBase);
		break;

	default:
		_stprintf(lpBuffer, _T("%d"), nNumber);
		break;
	}	
}

// 根据其基数将无符号整数转换为字符串表示
void IniManager::__UIntToString(UINT nNumber, LPTSTR lpBuffer, int nBase)
{
	switch (nBase)
	{
	case BASE_BINARY:
		__ToBinaryString(nNumber, lpBuffer, DEF_PROFILE_NUM_LEN);
		break;

	case BASE_OCTAL:
		_stprintf(lpBuffer, _T("%o"), nNumber);
		break;

	case BASE_HEXADECIMAL:
		_stprintf(lpBuffer, _T("%X"), nNumber);
		break;

	default:
		_stprintf(lpBuffer, _T("%u"), nNumber);
		break;
	}	
}

BOOL IniManager::StringToBool(LPCTSTR lpString, BOOL bDefault)
{
	// 默认值：空字符串
	// TRUE: "true", "yes", 非零十进制数
	// 错误：所有其他情况
	if (lpString == NULL || *lpString == _T('\0'))
		return bDefault;

	return (_tcsicmp(lpString, _T("true")) == 0
		|| _tcsicmp(lpString, _T("yes")) == 0
		|| _tcstol(lpString, NULL, BASE_DECIMAL) != 0);
}

BOOL IniManager::__TrimString(LPTSTR lpString)
{
	if (lpString == NULL)
		return FALSE;

	BOOL bTrimmed = FALSE;
	int nLen = _tcslen(lpString);

	// '\n' 和 '\r' 在这种情况下实际上是不可能的，但无论如何......
	// 修剪右侧
	while (nLen >= 0
		&& (lpString[nLen - 1] == _T(' ')
			|| lpString[nLen - 1] == _T('\t')
			|| lpString[nLen - 1] == _T('\r')
			|| lpString[nLen - 1] == _T('\n')))
	{
		lpString[--nLen] = _T('\0');
		bTrimmed = TRUE;		
	}

	// 修剪左侧
	LPCTSTR p = lpString; 
	while (*p == _T(' ')
			|| *p == _T('\t')
			|| *p == _T('\r')
			|| *p == _T('\n'))
	{
		p = &p[1];
		bTrimmed = TRUE;
	}

	if (p != lpString)
	{
		LPTSTR psz = _tcsdup(p);
		_tcscpy(lpString, psz);
		delete [] psz;
	}

	return bTrimmed;
}

LPTSTR IniManager::__StrDupEx(LPCTSTR lpStart, LPCTSTR lpEnd)
{
	const DWORD LEN = ((DWORD)lpEnd - (DWORD)lpStart) / sizeof(TCHAR);
	LPTSTR psz = new TCHAR[LEN + 1];
	_tcsncpy(psz, lpStart, LEN);
	psz[LEN] = _T('\0');
	return psz; //！！！ 需要调用者释放这个内存！！！
}

/////////////////////////////////////////////////////////////////////////////////
// Cini 类实现结束
/////////////////////////////////////////////////////////////////////////////////


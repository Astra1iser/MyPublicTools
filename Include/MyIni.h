#pragma once


//#include "stdafx.h"
//#include <windows.h>
//#include <tchar.h>
//#include <afxtempl.h>
// Number bases
#define BASE_BINARY			2
#define BASE_OCTAL			8
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

typedef BOOL (CALLBACK *SUBSTRPROC)(LPCTSTR, LPVOID);

class CIni
{
public:		

	//-----------------------------------------------------------
	//    构造 & 析构
	//-----------------------------------------------------------
	CIni(); // Default constructor
	CIni(LPCTSTR lpPathName); // Construct with a given file name
	virtual ~CIni();

	//-----------------------------------------------------------
	//    Ini 文件路径名访问
	//-----------------------------------------------------------
	void SetPathName(LPCTSTR lpPathName); // 指定新文件名

	CString GetPathName() const;

	
	//------------------------------------------------------------
	//    字符串访问
	//------------------------------------------------------------	
	DWORD GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault = NULL) const;
	//一般的获取字符串,用这个就好了
	CString GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault = NULL) const;

	BOOL WriteString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue) const;

	//从 ini 文件中读取一个字符串，附加另一个字符串，然后将其写回 ini 文件。
	BOOL AppendString(LPCTSTR Section, LPCTSTR lpKey, LPCTSTR lpString) const;
	
	//------------------------------------------------------------
	//    Ini 文件字符串数组访问
	//------------------------------------------------------------	
	// 解析从 ini 文件中检索到的字符串并将其拆分为一组子字符串。
	DWORD GetArray(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDelimiter = NULL, BOOL bTrimString = TRUE) const;
	//void GetArray(LPCTSTR lpSection, LPCTSTR lpKey, CStringArray* pArray, LPCTSTR lpDelimiter = NULL, BOOL bTrimString = TRUE) const;
	//BOOL WriteArray(LPCTSTR lpSection, LPCTSTR lpKey, const CStringArray* pArray, int nWriteCount = -1, LPCTSTR lpDelimiter = NULL) const;
	
	//------------------------------------------------------------
	//    原始数据类型访问
	//------------------------------------------------------------
	int GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase = BASE_DECIMAL) const;
	BOOL WriteInt(LPCTSTR lpSection, LPCTSTR lpKey, int nValue, int nBase = BASE_DECIMAL) const;
	BOOL IncreaseInt(LPCTSTR lpSection, LPCTSTR lpKey, int nIncrease = 1, int nBase = BASE_DECIMAL) const;
	
	UINT GetUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nDefault, int nBase = BASE_DECIMAL) const;
	BOOL WriteUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nValue, int nBase = BASE_DECIMAL) const;
	BOOL IncreaseUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nIncrease = 1, int nBase = BASE_DECIMAL) const;
	
	BOOL GetBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bDefault) const;
	BOOL WriteBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bValue) const;
	BOOL InvertBool(LPCTSTR lpSection, LPCTSTR lpKey) const;
	
	double GetDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fDefault) const;
	BOOL WriteDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fValue, int nPrecision = -1) const;
	BOOL IncreaseDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fIncrease, int nPrecision = -1) const;

	TCHAR GetChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR cDefault) const;
	BOOL WriteChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR c) const;

	//------------------------------------------------------------
	//    用户定义的数据类型和数据块访问
	//------------------------------------------------------------
	POINT GetPoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT ptDefault) const;
	BOOL WritePoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT pt) const;
	
	RECT GetRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rcDefault) const;
	BOOL WriteRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rc) const;

	DWORD GetDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPVOID lpBuffer, DWORD dwBufSize, DWORD dwOffset = 0) const;
	BOOL WriteDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const;
	BOOL AppendDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const;
	
	//------------------------------------------------------------
	//    部分操作
	//------------------------------------------------------------
	BOOL IsSectionExist(LPCTSTR lpSection) const;

	DWORD GetSectionNames(LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetSectionNames(CStringArray* pArray) const;

	BOOL CopySection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist) const;
	BOOL MoveSection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist = TRUE) const;
	BOOL DeleteSection(LPCTSTR lpSection) const;
	
	//------------------------------------------------------------
	//    键操作
	//------------------------------------------------------------
	BOOL IsKeyExist(LPCTSTR lpSection, LPCTSTR lpKey) const;	

	DWORD GetKeyLines(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetKeyLines(LPCTSTR lpSection, CStringArray* pArray) const;

	DWORD GetKeyNames(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetKeyNames(LPCTSTR lpSection, CStringArray* pArray) const;


	BOOL CopyKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist) const;
	BOOL MoveKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist = TRUE) const;
	BOOL DeleteKey(LPCTSTR lpSection, LPCTSTR lpKey) const;

	//------------------------------------------------------------
	// 解析“双空终止字符串”
	//------------------------------------------------------------
	static BOOL ParseDNTString(LPCTSTR lpString, SUBSTRPROC lpFnStrProc, LPVOID lpParam = NULL);

	//------------------------------------------------------------
	// 检查字符串是表示 TRUE 还是 FALSE
	//------------------------------------------------------------
	static BOOL StringToBool(LPCTSTR lpString, BOOL bDefault = FALSE);
		
protected:	

	//------------------------------------------------------------
	//    辅助函数
	//------------------------------------------------------------
	static LPTSTR __StrDupEx(LPCTSTR lpStart, LPCTSTR lpEnd);
	static BOOL __TrimString(LPTSTR lpBuffer);
	LPTSTR __GetStringDynamic(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault = NULL) const;
	static DWORD __StringSplit(LPCTSTR lpString, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDelimiter = NULL, BOOL bTrimString = TRUE);
	static void __ToBinaryString(UINT nNumber, LPTSTR lpBuffer, DWORD dwBufSize);
	static int __ValidateBase(int nBase);
	static void __IntToString(int nNumber, LPTSTR lpBuffer, int nBase);
	static void __UIntToString(UINT nNumber, LPTSTR lpBuffer, int nBase);
	static BOOL CALLBACK __SubStrCompare(LPCTSTR lpString1, LPVOID lpParam);
	static BOOL CALLBACK __KeyPairProc(LPCTSTR lpString, LPVOID lpParam);	

	static BOOL CALLBACK __SubStrAdd(LPCTSTR lpString, LPVOID lpParam);


	//------------------------------------------------------------
	//    成员变量
	//------------------------------------------------------------
	LPTSTR m_pszPathName; // 存储关联 ini 文件的路径
};

//#include "Ini.cpp"


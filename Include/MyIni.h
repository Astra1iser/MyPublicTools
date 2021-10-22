#pragma once


//#include "stdafx.h"
//#include <windows.h>
//#include <tchar.h>
//#include <afxtempl.h>
// 
// Number bases
#define BASE_BINARY			2
#define BASE_OCTAL			8
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

typedef BOOL (CALLBACK *SUBSTRPROC)(LPCTSTR, LPVOID);

class IniManager
{
public:		

	//-----------------------------------------------------------
	//    构造 & 析构
	//-----------------------------------------------------------
	IniManager(); // Default constructor
	IniManager(LPCTSTR lpPathName); // Construct with a given file name
	virtual ~IniManager();

	//-----------------------------------------------------------
	//    Ini 文件路径名访问
	//-----------------------------------------------------------
	void SetPathName(LPCTSTR lpPathName); // 指定新文件名

	//获取当前设置的INI文件路径
	CString GetPathName() const;

	
	//------------------------------------------------------------
	//    字符串访问
	//------------------------------------------------------------
	/*此函数一般作为辅助函数*/
	DWORD GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault = NULL) const;

	/*
	一般的获取字符串, 用这个就好了, 注意请不要使用UTF - 8的ini作为配置文件
	参数1:节点名
	参数2:键名
	参数3:找不到值返回的默认值
	*/
	CString GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault = NULL) const;

	/*
	写内容一般用这个函数,可以写中文,英文,数字
	参数1:节点名
	参数2:键名
	参数3:键值
	*/
	BOOL WriteString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue) const;

	/*
	从 ini 文件中读取一个字符串，在结尾附加另一个字符串，然后将其写回 ini 文件。
	参数1:节点名
	参数2:键名
	参数3:附加键值
	*/
	BOOL AppendString(LPCTSTR Section, LPCTSTR lpKey, LPCTSTR lpString) const;
	
	//------------------------------------------------------------
	//    Ini 文件字符串数组访问
	//------------------------------------------------------------	
	// 解析从 ini 文件中检索到的字符串并将其拆分为一组子字符串。
	//DWORD GetArray(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDelimiter = NULL, BOOL bTrimString = TRUE) const;
	//void GetArray(LPCTSTR lpSection, LPCTSTR lpKey, CStringArray* pArray, LPCTSTR lpDelimiter = NULL, BOOL bTrimString = TRUE) const;
	//BOOL WriteArray(LPCTSTR lpSection, LPCTSTR lpKey, const CStringArray* pArray, int nWriteCount = -1, LPCTSTR lpDelimiter = NULL) const;
	
	//------------------------------------------------------------
	//    原始数据类型访问
	//------------------------------------------------------------

	/*
	一般获取int类型的数据 用这个就好了,当然也可以用GetString()来获取,然后强转一下
	参数1:节点名
	参数2:键名
	参数3:键值
	*/
	int GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase = BASE_DECIMAL) const;

	/*
	写入int类型数据
	参数1:节点名
	参数2:键名
	参数3:键值
	*/
	BOOL WriteInt(LPCTSTR lpSection, LPCTSTR lpKey, int nValue, int nBase = BASE_DECIMAL) const;

	/*
	增加一个节点的int类型数据的值
	参数1:节点名
	参数2:键名
	参数3:要增加的值
	该节点的结果值等于当前值+参数3的值
	*/
	BOOL IncreaseInt(LPCTSTR lpSection, LPCTSTR lpKey, int nIncrease = 1, int nBase = BASE_DECIMAL) const;
	
	/*
	获取无符号整数,如果要获取的值为有符号整数,则返回一个无符号数(这个数是计算了符号后的结果,不是去掉符号的结果)
	参数1:节点名
	参数2:键名
	参数3:找不到这个节点值后返回的默认值
	*/
	UINT GetUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nDefault, int nBase = BASE_DECIMAL) const;

	/*
	写入无符号整数,如果要写入的值为有符号整数,则写入一个无符号数(这个数是计算了符号后的结果,不是去掉符号的结果)
	参数1:节点名
	参数2:键名
	参数3:键值
	*/
	BOOL WriteUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nValue, int nBase = BASE_DECIMAL) const;

	/*
	增加一个节点的无符号整数类型数据的值
	参数1:节点名
	参数2:键名
	参数3:要增加的值
	该节点的结果值等于当前值+参数3的值
	*/
	BOOL IncreaseUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nIncrease = 1, int nBase = BASE_DECIMAL) const;
	
	/*
	获取布尔类型的值
	参数1:节点名
	参数2:键名
	参数3:找不到该值后返回的默认值
	*/
	BOOL GetBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bDefault) const;

	/*
	写入布尔值
	参数1:节点名
	参数2:键名
	参数3:布尔值
	*/
	BOOL WriteBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bValue) const;

	/*
	反转布尔值并回写
	参数1:节点名
	参数2:键名
	*/
	BOOL InvertBool(LPCTSTR lpSection, LPCTSTR lpKey) const;
	
	/*
	获取双精度浮点数的值
	参数1:节点名
	参数2:键名
	参数3:找不到该值后返回的默认值
	*/
	double GetDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fDefault) const;

	/*
	写入双精度浮点型小数
	参数1:节点名
	参数2:键名
	参数3:键值
	参数4:精度,保留几位小数,如果没特殊需求,保持-1则保留6位小数
	*/
	BOOL WriteDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fValue, int nPrecision = -1) const;

	/*
	写入附加双精度浮点型小数
	参数1:节点名
	参数2:键名
	参数3:附加键值
	参数4:精度,保留几位小数,如果没特殊需求,保持-1则保留6位小数
	*/
	BOOL IncreaseDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fIncrease, int nPrecision = -1) const;

	/*
	获取单字符类型值(注意:如果找到的键值是个字符串则返回字符串的首字母)
	1.参数1:节点名
	2.参数2:键名
	3.参数3:找不到该值后返回的默认值
	*/
	TCHAR GetChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR cDefault) const;

	/*
	写入单字符类型值(注意:如果参数3是个字符串,则只会写入首字母字符)
	参数1:节点名
	参数2:键名
	参数3:找不到该值后返回的默认值
	*/
	BOOL WriteChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR c) const;

	//------------------------------------------------------------
	//    用户定义的数据类型和数据块访问
	//------------------------------------------------------------
	//POINT GetPoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT ptDefault) const;
	//BOOL WritePoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT pt) const;
	
	//RECT GetRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rcDefault) const;
	//BOOL WriteRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rc) const;

	//DWORD GetDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPVOID lpBuffer, DWORD dwBufSize, DWORD dwOffset = 0) const;
	//BOOL WriteDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const;
	//BOOL AppendDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const;
	
	//------------------------------------------------------------
	//    节点操作
	//------------------------------------------------------------

	/*
	节点是否存在
	参数:节点名
	*/
	BOOL IsSectionExist(LPCTSTR lpSection) const;

	/*
	此函数为辅助函数,请勿使用(获取有几个节点)
	*/
	DWORD GetSectionNames(LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetSectionNames(CStringArray* pArray) const;

	/*
	复制一个节点及节点下的所有键值到一个新节点
	参数1:源节点
	参数2:新节点名
	参数3:如果不存在时返回的布尔值
	*/
	BOOL CopySection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist) const;

	/*
	移动(重命名)节点
	参数1:源节点名
	参数2:目标节点名
	参数3:如果不存在时返回的布尔值
	*/
	BOOL MoveSection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist = TRUE) const;

	/*
	删除节点及节点下的键值
	参数:节点名
	*/
	BOOL DeleteSection(LPCTSTR lpSection) const;
	
	//------------------------------------------------------------
	//    键操作
	//------------------------------------------------------------

	/*
	判断键存不存在
	参数1:节点名
	参数2:键名
	*/
	BOOL IsKeyExist(LPCTSTR lpSection, LPCTSTR lpKey) const;	

	/*
	此函数为辅助函数,请勿使用(获取节点下键的行号)
	*/
	DWORD GetKeyLines(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetKeyLines(LPCTSTR lpSection, CStringArray* pArray) const;

	/*
	此函数为辅助函数,请勿使用(获取节点下有几个键)
	*/
	DWORD GetKeyNames(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetKeyNames(LPCTSTR lpSection, CStringArray* pArray) const;

	/*
	复制键和对应值到另一个节点下
	参数1:源节点名
	参数2:源键名
	参数3:目标节点名
	参数4:目标键名
	参数5:如果已经存在了则返回的布尔值
	*/
	BOOL CopyKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist) const;

	/*
	移动键和对应的值到另一个节点下
	参数1:源节点名
	参数2:源键名
	参数3:目标节点名
	参数4:目标键名
	参数5:如果已经存在了则返回的布尔值
	*/
	BOOL MoveKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist = TRUE) const;
	
	/*
	删除键及对应的值
	参数1:节点名
	参数2:键名
	*/
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

	//static BOOL CALLBACK __SubStrAdd(LPCTSTR lpString, LPVOID lpParam);


	//------------------------------------------------------------
	//    成员变量
	//------------------------------------------------------------
	LPTSTR m_pszPathName; // 存储关联 ini 文件的路径
};

//#include "Ini.cpp"


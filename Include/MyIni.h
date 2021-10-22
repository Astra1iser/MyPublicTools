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
	//    ���� & ����
	//-----------------------------------------------------------
	IniManager(); // Default constructor
	IniManager(LPCTSTR lpPathName); // Construct with a given file name
	virtual ~IniManager();

	//-----------------------------------------------------------
	//    Ini �ļ�·��������
	//-----------------------------------------------------------
	void SetPathName(LPCTSTR lpPathName); // ָ�����ļ���

	//��ȡ��ǰ���õ�INI�ļ�·��
	CString GetPathName() const;

	
	//------------------------------------------------------------
	//    �ַ�������
	//------------------------------------------------------------
	/*�˺���һ����Ϊ��������*/
	DWORD GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault = NULL) const;

	/*
	һ��Ļ�ȡ�ַ���, ������ͺ���, ע���벻Ҫʹ��UTF - 8��ini��Ϊ�����ļ�
	����1:�ڵ���
	����2:����
	����3:�Ҳ���ֵ���ص�Ĭ��ֵ
	*/
	CString GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault = NULL) const;

	/*
	д����һ�����������,����д����,Ӣ��,����
	����1:�ڵ���
	����2:����
	����3:��ֵ
	*/
	BOOL WriteString(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue) const;

	/*
	�� ini �ļ��ж�ȡһ���ַ������ڽ�β������һ���ַ�����Ȼ����д�� ini �ļ���
	����1:�ڵ���
	����2:����
	����3:���Ӽ�ֵ
	*/
	BOOL AppendString(LPCTSTR Section, LPCTSTR lpKey, LPCTSTR lpString) const;
	
	//------------------------------------------------------------
	//    Ini �ļ��ַ����������
	//------------------------------------------------------------	
	// ������ ini �ļ��м��������ַ�����������Ϊһ�����ַ�����
	//DWORD GetArray(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDelimiter = NULL, BOOL bTrimString = TRUE) const;
	//void GetArray(LPCTSTR lpSection, LPCTSTR lpKey, CStringArray* pArray, LPCTSTR lpDelimiter = NULL, BOOL bTrimString = TRUE) const;
	//BOOL WriteArray(LPCTSTR lpSection, LPCTSTR lpKey, const CStringArray* pArray, int nWriteCount = -1, LPCTSTR lpDelimiter = NULL) const;
	
	//------------------------------------------------------------
	//    ԭʼ�������ͷ���
	//------------------------------------------------------------

	/*
	һ���ȡint���͵����� ������ͺ���,��ȻҲ������GetString()����ȡ,Ȼ��ǿתһ��
	����1:�ڵ���
	����2:����
	����3:��ֵ
	*/
	int GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase = BASE_DECIMAL) const;

	/*
	д��int��������
	����1:�ڵ���
	����2:����
	����3:��ֵ
	*/
	BOOL WriteInt(LPCTSTR lpSection, LPCTSTR lpKey, int nValue, int nBase = BASE_DECIMAL) const;

	/*
	����һ���ڵ��int�������ݵ�ֵ
	����1:�ڵ���
	����2:����
	����3:Ҫ���ӵ�ֵ
	�ýڵ�Ľ��ֵ���ڵ�ǰֵ+����3��ֵ
	*/
	BOOL IncreaseInt(LPCTSTR lpSection, LPCTSTR lpKey, int nIncrease = 1, int nBase = BASE_DECIMAL) const;
	
	/*
	��ȡ�޷�������,���Ҫ��ȡ��ֵΪ�з�������,�򷵻�һ���޷�����(������Ǽ����˷��ź�Ľ��,����ȥ�����ŵĽ��)
	����1:�ڵ���
	����2:����
	����3:�Ҳ�������ڵ�ֵ�󷵻ص�Ĭ��ֵ
	*/
	UINT GetUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nDefault, int nBase = BASE_DECIMAL) const;

	/*
	д���޷�������,���Ҫд���ֵΪ�з�������,��д��һ���޷�����(������Ǽ����˷��ź�Ľ��,����ȥ�����ŵĽ��)
	����1:�ڵ���
	����2:����
	����3:��ֵ
	*/
	BOOL WriteUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nValue, int nBase = BASE_DECIMAL) const;

	/*
	����һ���ڵ���޷��������������ݵ�ֵ
	����1:�ڵ���
	����2:����
	����3:Ҫ���ӵ�ֵ
	�ýڵ�Ľ��ֵ���ڵ�ǰֵ+����3��ֵ
	*/
	BOOL IncreaseUInt(LPCTSTR lpSection, LPCTSTR lpKey, UINT nIncrease = 1, int nBase = BASE_DECIMAL) const;
	
	/*
	��ȡ�������͵�ֵ
	����1:�ڵ���
	����2:����
	����3:�Ҳ�����ֵ�󷵻ص�Ĭ��ֵ
	*/
	BOOL GetBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bDefault) const;

	/*
	д�벼��ֵ
	����1:�ڵ���
	����2:����
	����3:����ֵ
	*/
	BOOL WriteBool(LPCTSTR lpSection, LPCTSTR lpKey, BOOL bValue) const;

	/*
	��ת����ֵ����д
	����1:�ڵ���
	����2:����
	*/
	BOOL InvertBool(LPCTSTR lpSection, LPCTSTR lpKey) const;
	
	/*
	��ȡ˫���ȸ�������ֵ
	����1:�ڵ���
	����2:����
	����3:�Ҳ�����ֵ�󷵻ص�Ĭ��ֵ
	*/
	double GetDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fDefault) const;

	/*
	д��˫���ȸ�����С��
	����1:�ڵ���
	����2:����
	����3:��ֵ
	����4:����,������λС��,���û��������,����-1����6λС��
	*/
	BOOL WriteDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fValue, int nPrecision = -1) const;

	/*
	д�븽��˫���ȸ�����С��
	����1:�ڵ���
	����2:����
	����3:���Ӽ�ֵ
	����4:����,������λС��,���û��������,����-1����6λС��
	*/
	BOOL IncreaseDouble(LPCTSTR lpSection, LPCTSTR lpKey, double fIncrease, int nPrecision = -1) const;

	/*
	��ȡ���ַ�����ֵ(ע��:����ҵ��ļ�ֵ�Ǹ��ַ����򷵻��ַ���������ĸ)
	1.����1:�ڵ���
	2.����2:����
	3.����3:�Ҳ�����ֵ�󷵻ص�Ĭ��ֵ
	*/
	TCHAR GetChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR cDefault) const;

	/*
	д�뵥�ַ�����ֵ(ע��:�������3�Ǹ��ַ���,��ֻ��д������ĸ�ַ�)
	����1:�ڵ���
	����2:����
	����3:�Ҳ�����ֵ�󷵻ص�Ĭ��ֵ
	*/
	BOOL WriteChar(LPCTSTR lpSection, LPCTSTR lpKey, TCHAR c) const;

	//------------------------------------------------------------
	//    �û�������������ͺ����ݿ����
	//------------------------------------------------------------
	//POINT GetPoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT ptDefault) const;
	//BOOL WritePoint(LPCTSTR lpSection, LPCTSTR lpKey, POINT pt) const;
	
	//RECT GetRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rcDefault) const;
	//BOOL WriteRect(LPCTSTR lpSection, LPCTSTR lpKey, RECT rc) const;

	//DWORD GetDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPVOID lpBuffer, DWORD dwBufSize, DWORD dwOffset = 0) const;
	//BOOL WriteDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const;
	//BOOL AppendDataBlock(LPCTSTR lpSection, LPCTSTR lpKey, LPCVOID lpData, DWORD dwDataSize) const;
	
	//------------------------------------------------------------
	//    �ڵ����
	//------------------------------------------------------------

	/*
	�ڵ��Ƿ����
	����:�ڵ���
	*/
	BOOL IsSectionExist(LPCTSTR lpSection) const;

	/*
	�˺���Ϊ��������,����ʹ��(��ȡ�м����ڵ�)
	*/
	DWORD GetSectionNames(LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetSectionNames(CStringArray* pArray) const;

	/*
	����һ���ڵ㼰�ڵ��µ����м�ֵ��һ���½ڵ�
	����1:Դ�ڵ�
	����2:�½ڵ���
	����3:���������ʱ���صĲ���ֵ
	*/
	BOOL CopySection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist) const;

	/*
	�ƶ�(������)�ڵ�
	����1:Դ�ڵ���
	����2:Ŀ��ڵ���
	����3:���������ʱ���صĲ���ֵ
	*/
	BOOL MoveSection(LPCTSTR lpSrcSection, LPCTSTR lpDestSection, BOOL bFailIfExist = TRUE) const;

	/*
	ɾ���ڵ㼰�ڵ��µļ�ֵ
	����:�ڵ���
	*/
	BOOL DeleteSection(LPCTSTR lpSection) const;
	
	//------------------------------------------------------------
	//    ������
	//------------------------------------------------------------

	/*
	�жϼ��治����
	����1:�ڵ���
	����2:����
	*/
	BOOL IsKeyExist(LPCTSTR lpSection, LPCTSTR lpKey) const;	

	/*
	�˺���Ϊ��������,����ʹ��(��ȡ�ڵ��¼����к�)
	*/
	DWORD GetKeyLines(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetKeyLines(LPCTSTR lpSection, CStringArray* pArray) const;

	/*
	�˺���Ϊ��������,����ʹ��(��ȡ�ڵ����м�����)
	*/
	DWORD GetKeyNames(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD dwBufSize) const;
	//void GetKeyNames(LPCTSTR lpSection, CStringArray* pArray) const;

	/*
	���Ƽ��Ͷ�Ӧֵ����һ���ڵ���
	����1:Դ�ڵ���
	����2:Դ����
	����3:Ŀ��ڵ���
	����4:Ŀ�����
	����5:����Ѿ��������򷵻صĲ���ֵ
	*/
	BOOL CopyKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist) const;

	/*
	�ƶ����Ͷ�Ӧ��ֵ����һ���ڵ���
	����1:Դ�ڵ���
	����2:Դ����
	����3:Ŀ��ڵ���
	����4:Ŀ�����
	����5:����Ѿ��������򷵻صĲ���ֵ
	*/
	BOOL MoveKey(LPCTSTR lpSrcSection, LPCTSTR lpSrcKey, LPCTSTR lpDestSection, LPCTSTR lpDestKey, BOOL bFailIfExist = TRUE) const;
	
	/*
	ɾ��������Ӧ��ֵ
	����1:�ڵ���
	����2:����
	*/
	BOOL DeleteKey(LPCTSTR lpSection, LPCTSTR lpKey) const;

	//------------------------------------------------------------
	// ������˫����ֹ�ַ�����
	//------------------------------------------------------------
	static BOOL ParseDNTString(LPCTSTR lpString, SUBSTRPROC lpFnStrProc, LPVOID lpParam = NULL);

	//------------------------------------------------------------
	// ����ַ����Ǳ�ʾ TRUE ���� FALSE
	//------------------------------------------------------------
	static BOOL StringToBool(LPCTSTR lpString, BOOL bDefault = FALSE);
		
protected:	

	//------------------------------------------------------------
	//    ��������
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
	//    ��Ա����
	//------------------------------------------------------------
	LPTSTR m_pszPathName; // �洢���� ini �ļ���·��
};

//#include "Ini.cpp"


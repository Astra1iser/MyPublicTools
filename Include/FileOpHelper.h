#ifndef _FILE_OP_HELPER_
#define _FILE_OP_HELPER_


namespace FileOpHelper
{
	CString GetExeVer(const CString& strFile);
	BOOL GetPEFileVer(const CString& strFile, CString& strVer);
	BOOL GetPEFileVer(const CString& strFile, DWORD& dwHight, DWORD& dwLow);
	VOID VersionStringToNum(LPCTSTR pVersion, DWORD *pMS, DWORD *pLS);

	BOOL GetFileVersionCommentInfo(LPCTSTR strFile, CString& strInfo);
	void GetFileVersionDetailInfo(LPCTSTR szFullPath,CString& strFileDesc,CString& strProductDesc,CString& strCompany);

	BOOL GetFileMD5String(const CString& strFile, CString& strMD5Result, int nLen);
	BOOL GetFileFromRes(UINT nResID, LPCTSTR lpcszFile);
	BOOL GetFileFromRes(LPCTSTR lpSrcFile,LPCTSTR lpResType,int nResID, LPCTSTR lpDstFile);
	BOOL SafeWriteToFile(LPCTSTR pcsDstFile, BYTE* bySrc, DWORD dwSrcLen);

	BOOL SafeDeleteFile(LPCTSTR lpFile);			// �������ƶ���ɾ������ֹ������״̬�޷�ֱ��ɾ��
	BOOL SafeCopyFile(LPCTSTR src, LPCTSTR dst);	// ��ȫ�����ļ���ֱ��CopyFile�����fileid�仯�������޷����ұ���

	// �������滻PE�ļ�����ΪҪ��QAXǩ������PE�ļ���ֱ��ʹ��SafeCopyFile
	int ReplaceFile(LPCTSTR lpSrcFile,LPCTSTR lpDstFile,BOOL bCheckVer = FALSE,BOOL bCheckComments = FALSE,LPCTSTR lpComments = L"",LPCTSTR lpBackupName = L"");
	int ReplaceFileByRes(DWORD dwResID,LPCTSTR lpSrcMD5,DWORD dwResVerHigh,DWORD dwResVerLow,LPCTSTR lpDstFile,BOOL bCheckVer = FALSE,BOOL bCheckComments = FALSE,
		LPCTSTR lpComments = L"",LPCTSTR lpBackupName = L"");

	BOOL DeleteDir(LPCTSTR lpDirPath);
	BOOL IsPEFile(PCTSTR pFileName, BOOL& bPE);

	BOOL GetSignName(CString& strFile, CString& strSignName);

	// �Ƚ������汾,����ֵԼ�����ַ����Ƚ�һ����sepΪ�ָ�����Ĭ��Ϊ.��ָ��Ϊ����ʱ�ɱȽ�������ʽ�Ĵ�����2012-08-15����
	int CompareVersion(CString ver1,CString ver2,const CString& strSep1 = L".",const CString& strSep2 = L".");
};












#endif
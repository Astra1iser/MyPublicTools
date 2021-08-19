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

	BOOL SafeDeleteFile(LPCTSTR lpFile);			// 改名、移动、删除，防止被加载状态无法直接删除
	BOOL SafeCopyFile(LPCTSTR src, LPCTSTR dst);	// 安全复制文件，直接CopyFile会出现fileid变化，引起无法自我保护

	// 仅用于替换PE文件，因为要验QAX签名，非PE文件可直接使用SafeCopyFile
	int ReplaceFile(LPCTSTR lpSrcFile,LPCTSTR lpDstFile,BOOL bCheckVer = FALSE,BOOL bCheckComments = FALSE,LPCTSTR lpComments = L"",LPCTSTR lpBackupName = L"");
	int ReplaceFileByRes(DWORD dwResID,LPCTSTR lpSrcMD5,DWORD dwResVerHigh,DWORD dwResVerLow,LPCTSTR lpDstFile,BOOL bCheckVer = FALSE,BOOL bCheckComments = FALSE,
		LPCTSTR lpComments = L"",LPCTSTR lpBackupName = L"");

	BOOL DeleteDir(LPCTSTR lpDirPath);
	BOOL IsPEFile(PCTSTR pFileName, BOOL& bPE);

	BOOL GetSignName(CString& strFile, CString& strSignName);

	// 比较两个版本,返回值约定和字符串比较一样，sep为分隔符，默认为.，指定为其他时可比较其他形式的串，如2012-08-15这种
	int CompareVersion(CString ver1,CString ver2,const CString& strSep1 = L".",const CString& strSep2 = L".");
};












#endif
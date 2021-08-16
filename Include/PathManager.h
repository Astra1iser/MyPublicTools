#ifndef _PATH_MANAGER_H_
#define _PATH_MANAGER_H_

/*
	路径相关函数
*/

namespace PathManager
{
	//获取当前运行程序的全路径包括运行文件的后缀
	CString MyExeFullPath();

	// 只获取当前运行的exe名字，参数为是否去掉.exe扩展名
	CString MyExeName(BOOL bRemoveExt);

	//返回当前程序运行的路径并拼合strCombinePath参数,结尾不带"/"
	CString MyExePath(LPCTSTR strCombinePath);

	//拼合两个路径 结果 : strParentPath/strCombinePath
	CString MyCombinePath(LPCTSTR strParentPath, LPCTSTR strCombinePath);

	//获取某个句柄所在目录,strCombinePath参数为可拼接的路径
	CString GetCurModulePath(HMODULE hMod, LPCTSTR strCombinePath);

	//提取路径中到末尾的文件名字,如果是获取带后缀的文件,bNeedExtName应置为1
	CString GetFileNameFromPath(CString strPath, BOOL bNeedExtName);

	//获取注册表的某个键的值是否是文件夹 参数1可以为
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS  
	//参数2是具体路径,如
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//参数3是键名,如
		// L"Path"
	//返回为空则证明文件夹不存在或者注册表不存在
	CString GetProgramInstallPath(HKEY RegDir, LPCTSTR strRegPath, LPCTSTR strKeyName);

	//创建一个文件夹,TopisDir是指strDir的结尾是否不带"\\或者/"若strDir结尾是"\\或者/"则TopisDir应该为FALSE,否则为TRUE
	BOOL CreateDirRecursive( CString strDir, BOOL TopisDir = TRUE );
};

#endif
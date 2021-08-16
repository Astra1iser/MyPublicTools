#ifndef _PATH_MANAGER_H_
#define _PATH_MANAGER_H_

/*
	·����غ���
*/

namespace PathManager
{
	//��ȡ��ǰ���г����ȫ·�����������ļ��ĺ�׺
	CString MyExeFullPath();

	// ֻ��ȡ��ǰ���е�exe���֣�����Ϊ�Ƿ�ȥ��.exe��չ��
	CString MyExeName(BOOL bRemoveExt);

	//���ص�ǰ�������е�·����ƴ��strCombinePath����,��β����"/"
	CString MyExePath(LPCTSTR strCombinePath);

	//ƴ������·�� ��� : strParentPath/strCombinePath
	CString MyCombinePath(LPCTSTR strParentPath, LPCTSTR strCombinePath);

	//��ȡĳ���������Ŀ¼,strCombinePath����Ϊ��ƴ�ӵ�·��
	CString GetCurModulePath(HMODULE hMod, LPCTSTR strCombinePath);

	//��ȡ·���е�ĩβ���ļ�����,����ǻ�ȡ����׺���ļ�,bNeedExtNameӦ��Ϊ1
	CString GetFileNameFromPath(CString strPath, BOOL bNeedExtName);

	//��ȡע����ĳ������ֵ�Ƿ����ļ��� ����1����Ϊ
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS  
	//����2�Ǿ���·��,��
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//����3�Ǽ���,��
		// L"Path"
	//����Ϊ����֤���ļ��в����ڻ���ע�������
	CString GetProgramInstallPath(HKEY RegDir, LPCTSTR strRegPath, LPCTSTR strKeyName);

	//����һ���ļ���,TopisDir��ָstrDir�Ľ�β�Ƿ񲻴�"\\����/"��strDir��β��"\\����/"��TopisDirӦ��ΪFALSE,����ΪTRUE
	BOOL CreateDirRecursive( CString strDir, BOOL TopisDir = TRUE );
};

#endif
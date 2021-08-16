#pragma once

namespace RegeditManager {

	//ע��:�˷�������ȡ�ù���ԱȨ��
	//����ע����е�ֵ
	//ʹ�÷��� RegeditManager::setReg(��Ŀ¼, ��Ŀ¼��, ����, ֵ);
	//��ȡע����ĳ������ֵ�Ƿ����ļ��� ����1����Ϊ
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS  
	//����2�Ǿ���·��,��
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//����3�Ǽ���,��
		// L"Path"
	//����4��ֵ
	//����5Ϊд������ TRUEΪstr���� FALSEΪDWORD����
	BOOL SetReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName, const std::wstring& value, BOOL isStrOrInt);

	//ע��:�˷�������ȡ�ù���ԱȨ��
	//���ע�����str���͵ļ���ֵ
	//ʹ�÷���		auto t = GetStrReg(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe", L"Path");
	//				wcout << get<1>(t) << endl;
	//				get<0>(t)Ϊִ���Ƿ�ɹ� 1�ɹ� 0ʧ��
	//����1����Ϊ
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS 
	//����2�Ǿ���·��,��
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//����3�Ǽ���,��
		// L"Path"
	std::tuple<bool, std::wstring> GetStrReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName);

	//ע��:�˷�������ȡ�ù���ԱȨ��
	//���ע�����DWORD���͵ļ���ֵ
	//ʹ�÷���		auto t = GetIntReg(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe", L"Path");
	//				wcout << get<1>(t) << endl;
	//				get<0>(t)Ϊִ���Ƿ�ɹ� 1�ɹ� 0ʧ��
	//����1����Ϊ
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS 
	//����2�Ǿ���·��,��
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//����3�Ǽ���,��
		// L"Path"
	std::tuple<bool, DWORD> GetIntReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName);

	//ע��:�˷�������ȡ�ù���ԱȨ��
	//ɾ��ע����е������
	//ʹ�÷��� RegeditManager::DeleteReg(��Ŀ¼, ��Ŀ¼��, ����);
	//��ȡע����ĳ������ֵ�Ƿ����ļ��� ����1����Ϊ
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS  
	//����2�Ǿ���·��,��
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//����3�Ǽ���,��
		// L"Path"
	BOOL DeleteReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName);
}

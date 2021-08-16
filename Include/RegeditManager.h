#pragma once

namespace RegeditManager {

	//注意:此方法必须取得管理员权限
	//设置注册表中的值
	//使用方法 RegeditManager::setReg(主目录, 子目录树, 键名, 值);
	//获取注册表的某个键的值是否是文件夹 参数1可以为
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS  
	//参数2是具体路径,如
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//参数3是键名,如
		// L"Path"
	//参数4是值
	//参数5为写入类型 TRUE为str类型 FALSE为DWORD类型
	BOOL SetReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName, const std::wstring& value, BOOL isStrOrInt);

	//注意:此方法必须取得管理员权限
	//获得注册表中str类型的键的值
	//使用方法		auto t = GetStrReg(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe", L"Path");
	//				wcout << get<1>(t) << endl;
	//				get<0>(t)为执行是否成功 1成功 0失败
	//参数1可以为
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS 
	//参数2是具体路径,如
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//参数3是键名,如
		// L"Path"
	std::tuple<bool, std::wstring> GetStrReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName);

	//注意:此方法必须取得管理员权限
	//获得注册表中DWORD类型的键的值
	//使用方法		auto t = GetIntReg(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe", L"Path");
	//				wcout << get<1>(t) << endl;
	//				get<0>(t)为执行是否成功 1成功 0失败
	//参数1可以为
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS 
	//参数2是具体路径,如
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//参数3是键名,如
		// L"Path"
	std::tuple<bool, DWORD> GetIntReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName);

	//注意:此方法必须取得管理员权限
	//删除注册表中的任意键
	//使用方法 RegeditManager::DeleteReg(主目录, 子目录树, 键名);
	//获取注册表的某个键的值是否是文件夹 参数1可以为
		// HKEY_CLASSES_ROOT         
		// HKEY_CURRENT_USER 
		// HKEY_LOCAL_MACHINE
		// HKEY_USERS  
	//参数2是具体路径,如
		// L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe"
	//参数3是键名,如
		// L"Path"
	BOOL DeleteReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName);
}

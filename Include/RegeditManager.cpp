#include "stdafx.h"
#include "RegeditManager.h"

BOOL RegeditManager::SetReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName, const std::wstring& value,BOOL isStrOrInt)
{
	if (keyName.empty() || value.empty()) {
		return FALSE;
	}

	CRegKey key;

	if (ERROR_SUCCESS == key.Create(RegDir, keyName.c_str()))
	{
		auto setres = TRUE;
		if (TRUE == isStrOrInt)
		{
			setres = key.SetStringValue(valueName.c_str(), value.c_str());
			key.Close();
			return TRUE;
		}
		else if (FALSE == isStrOrInt)
		{
			setres = key.SetDWORDValue(valueName.c_str(), _ttoi(value.c_str()));
			key.Close();
			return TRUE;
		}

		if (setres != ERROR_SUCCESS)
		{
			//wcout<<(L"REG Set url reg:%s error, key.SetStringValue GetLastError():%d", valueName.c_str(), GetLastError());
			return FALSE;
		}
		else
		{
			//wcout<<(L"REG Set url reg:%s ,sucess value: %s", valueName.c_str(), value.c_str());
			return TRUE;
		}
	}
	else 
	{
		//wcout<<(L"REG Set url reg:%s error, key.create GetLastError(): %d", valueName.c_str(), GetLastError());
		return FALSE;
	}

	key.Close();
	return FALSE;
}

std::tuple<bool, std::wstring> RegeditManager::GetStrReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName)
{
	CRegKey key;
	ULONG len = MAX_PATH;
	WCHAR szValue[MAX_PATH] = { 0 };

	if (ERROR_SUCCESS == key.Open(RegDir, keyName.c_str()))
	{
		LSTATUS lst = key.QueryStringValue(valueName.c_str(), szValue, &len);
		if (ERROR_SUCCESS == lst)
		{
			key.Close();
			return std::make_tuple(true, szValue);
		}
		else
		{
			//wcout << (L"REG QueryStringValue fail, key: %s ,mvalue name: %s, GetLastError:%d", keyName, valueName, GetLastError());
			return std::make_tuple(false, L"");
		}		
	}
	key.Close();

	//wcout<<(L"REG open fail, key: %s, value name: %s, GetLastError:%d", keyName, valueName, GetLastError());
	return std::make_tuple(false, L"");
}

std::tuple<bool, DWORD> RegeditManager::GetIntReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName)
{
	CRegKey key;
	ULONG len = MAX_PATH;
	DWORD szValue = 0;
	if (ERROR_SUCCESS == key.Open(RegDir, keyName.c_str()))
	{
		LSTATUS lst = key.QueryDWORDValue(valueName.c_str(), szValue);
		if (ERROR_SUCCESS == lst)
		{
			key.Close();
			return std::make_tuple(true, szValue);
		}
		else
		{
			//wcout << (L"REG QueryStringValue fail, key: %s ,mvalue name: %s, GetLastError:%d", keyName, valueName, GetLastError());
			return std::make_tuple(false, 0);
		}
	}
	//wcout << (L"REG open fail, key: %s, value name: %s, GetLastError:%d", keyName, valueName, GetLastError());
	return std::make_tuple(false, 0);
}

BOOL RegeditManager::DeleteReg(HKEY RegDir, const std::wstring& keyName, const std::wstring& valueName)
{
	CRegKey key;
	if (ERROR_SUCCESS == key.Create(RegDir, keyName.c_str())) {
		key.DeleteValue(valueName.c_str());
		key.Close();
		//wcout<<(L"REG Delete url reg success: %s", valueName.c_str());
		return TRUE;
	}
	else {
		//wcout<<(L"REG Delete url reg fail: %s, error code: %d", valueName.c_str(),GetLastError());
		key.Close();
		return FALSE;
	}
	key.Close();
	return FALSE;
}

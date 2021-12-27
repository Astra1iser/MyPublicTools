#include "stdafx.h"
#include "CorrectInstalledPatches.h"
#include <atlfile.h>

#include <comdef.h>
#include <WbemCli.h>
#pragma comment(lib, "wbemuuid.lib")

CCorrectInstalledPatches::CCorrectInstalledPatches()
{

}

void  _SplitStringVec(LPTSTR pString,std::vector<CString>& vec_rlt,const CString& strSep/* = L";"*/)
{
	vec_rlt.clear();

	if (!pString)
	{
		return;
	}

	LPTSTR pSubPtr = pString;
	LPTSTR pTemp   = pSubPtr;

	while(pTemp)
	{
		pTemp = wcsstr(pSubPtr,strSep.GetString());

		if(pTemp)
		{
			*pTemp = L'\0';
			vec_rlt.push_back(pSubPtr);
			pSubPtr = ++pTemp;
		}
		else
		{
			if (wcslen(pSubPtr))
			{
				vec_rlt.push_back(pSubPtr);
			}
			break;
		}
	}
}

CCorrectInstalledPatches::~CCorrectInstalledPatches()
{

}

bool CCorrectInstalledPatches::GetInstalledPatches(std::map<DWORD,CString>& wmiResult)
{
	bool bRet = false;

	IWbemLocator *pLoc = NULL;

	IWbemServices *pSvc = NULL;
	/*
		对于每个使用com库的线程，CoInitializeEx 至少必须调用一次,通常也只调用一次。
		只要传递相同的并发标志参数，同一个线程多次调用CoInitializeEx也是允许的，但是后面的有效调用返回S_FLASE。 
		要在一个线程上合理关闭com库，每一次成功调用CoInitialize或者CoInitializeEx (包括任何返回S_FALSE的调用)，都必须对应调用一次CoUninitialize。
		https://blog.csdn.net/haizhongyun/article/details/7625625
	*/

	//DB_LOG_DEBUGW(L"开始调用WMI接口获取已安装补丁");
	 CoInitializeEx(0, COINIT_MULTITHREADED); 

	do
	{
		HRESULT hres;


		hres =  CoInitializeSecurity(
			NULL, 
			-1,                          // COM authentication
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities 
			NULL                         // Reserved
			);


		if (!(SUCCEEDED(hres) || RPC_E_TOO_LATE == hres))
		{
			//DB_LOG_ERRORW(L"Failed to create IWbemLocator object. Error code = 0X%0X", hres);
			break;                    // Program has failed.
		}

		hres = CoCreateInstance(
			CLSID_WbemLocator,             
			0, 
			CLSCTX_INPROC_SERVER, 
			IID_IWbemLocator, (LPVOID *) &pLoc);

		if (FAILED(hres))
		{
			//DB_LOG_ERRORW(L"Failed to create IWbemLocator object. Error code = 0X%0X", hres);
			break;// Program has failed.
		}


		hres = pLoc->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
			NULL,                    // User name. NULL = current user
			NULL,                    // User password. NULL = current
			0,                       // Locale. NULL indicates current
			NULL,                    // Security flags.
			0,                       // Authority (for example, Kerberos)
			0,                       // Context object 
			&pSvc                    // pointer to IWbemServices proxy
			);

		if (FAILED(hres))
		{
			//DB_LOG_ERRORW(L"Could not connect. Error code = 0X%0X", hres);   
			break;// Program has failed.
		}


		hres = CoSetProxyBlanket(
			pSvc,                        // Indicates the proxy to set
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
			NULL,                        // Server principal name 
			RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
			NULL,                        // client identity
			EOAC_NONE                    // proxy capabilities 
			);

		if (FAILED(hres))
		{
			//DB_LOG_ERRORW(L"Could not set proxy blanket. Error code = 0X%0X", hres);
			break;// Program has failed.
		}


		IEnumWbemClassObject* pEnumerator = NULL;
		hres = pSvc->ExecQuery(
			bstr_t("WQL"), 
			bstr_t("SELECT * FROM Win32_QuickFixEngineering"),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			//DB_LOG_ERRORW(L"Failed to ExecQuery WQL. Error code = 0X%0X", hres);
			break;
		}

		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;

		std::map<CString,CString> mapInstalled;
		while (pEnumerator)
		{
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
				&pclsObj, &uReturn);

			if (FAILED(hr) || (!pclsObj))			
			{
				break;
			}

			if(0 == uReturn || S_FALSE == hr)
			{
				break;
			}

			_variant_t vtPropID;
			_variant_t vtPropDate;
			// Get the value of the HotFixID property
			hr = pclsObj->Get(L"HotFixID", 0, vtPropID.GetAddress(), 0, 0);
			if (FAILED(hr))			
			{
				pclsObj->Release();
				continue;
			}

			CString strID= (LPCTSTR)_bstr_t(vtPropID);
			strID.TrimLeft(L"KBkb");
			DWORD dwKey = (DWORD)_ttoi(strID);
		
			// Get the value of the InstalledOn property
			pclsObj->Get(L"InstalledOn", 0, vtPropDate.GetAddress(), 0, 0);

			CString strDate= (LPCTSTR)_bstr_t(vtPropDate);
			vector<CString> vecDate;
			_SplitStringVec(strDate.GetBuffer(),vecDate,L"/");
			if (vecDate.size() >=3)
			{
				strDate.Format(L"%d-%02d-%02d", _ttoi(vecDate[2]),_ttoi(vecDate[0]),_ttoi(vecDate[1]));
			}
			else
			{
				strDate.Empty();
			}
			wmiResult.insert(make_pair(dwKey, strDate));

			pclsObj->Release();
		}

		pEnumerator->Release();
		
		bRet = true;

	}while(false);

	if (pSvc)
	{
		pSvc->Release();
	}
	if (pLoc)
	{
		pLoc->Release();   
	}

	CoUninitialize();
	//DB_LOG_DEBUGW(L"调用WMI接口获取已安装补丁完成[%d]", wmiResult.size());
	
	return bRet;
}
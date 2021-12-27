#include "StdAfx.h"
#include "WinFirewallControler.h"

BOOL CWinFirewallController::AddPort(UINT nPort,NET_FW_IP_PROTOCOL protocol,CString strName)
{
	HRESULT hr = S_OK;
	HRESULT comInit = E_FAIL;
	INetFwProfile* fwProfile = NULL;

	do 
	{
		comInit = CoInitializeEx(0,COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (comInit != RPC_E_CHANGED_MODE)
		{
			hr = comInit;
			if (FAILED(hr))
				break;
		}

		hr = WindowsFirewallInitialize(&fwProfile);
		if (FAILED(hr))
			break;

		hr = WindowsFirewallAddPort(fwProfile,nPort,protocol,strName);
		if (FAILED(hr))
			break;

	} while (FALSE);

	WindowsFirewallCleanup(fwProfile);

	if (SUCCEEDED(comInit))
		CoUninitialize();
	
	return hr == S_OK;
}

BOOL CWinFirewallController::AddApp( CString strAppPath ,CString strName)
{
	HRESULT hr = S_OK;
	HRESULT comInit = E_FAIL;
	INetFwProfile* fwProfile = NULL;

	do 
	{
		comInit = CoInitializeEx(0,COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (comInit != RPC_E_CHANGED_MODE)
		{
			hr = comInit;
			if (FAILED(hr))
				break;
		}

		hr = WindowsFirewallInitialize(&fwProfile);
		if (FAILED(hr))
			break;

		hr = WindowsFirewallAddApp(fwProfile,strAppPath,strName);
		if (FAILED(hr))
			break;

	} while (FALSE);

	WindowsFirewallCleanup(fwProfile);

	if (SUCCEEDED(comInit))
		CoUninitialize();

	return hr == S_OK;
}

HRESULT CWinFirewallController::WindowsFirewallInitialize(OUT INetFwProfile** fwProfile)
{
	_ASSERT(fwProfile != NULL);
	*fwProfile = NULL;

	HRESULT hr = S_OK;
	INetFwMgr* fwMgr = NULL;
	INetFwPolicy* fwPolicy = NULL;
	do
	{
		hr = CoCreateInstance(__uuidof(NetFwMgr),NULL,CLSCTX_INPROC_SERVER,__uuidof(INetFwMgr),(void**)&fwMgr);
		if (FAILED(hr))
			break;

		hr = fwMgr->get_LocalPolicy(&fwPolicy);
		if (FAILED(hr))
			break;

		hr = fwPolicy->get_CurrentProfile(fwProfile);
		if (FAILED(hr))
			break;

	}while(0);

	if (fwPolicy != NULL)
		fwPolicy->Release();

	if (fwMgr != NULL)
		fwMgr->Release();

	return hr;
}

HRESULT CWinFirewallController::WindowsFirewallAddApp(IN INetFwProfile* fwProfile,IN const wchar_t* fwProcessImageFileName,IN const wchar_t* fwName)
{
	_ASSERT(fwProfile != NULL);
	_ASSERT(fwProcessImageFileName != NULL);
	_ASSERT(fwName != NULL);

	HRESULT hr = S_OK;
	BSTR fwBstrName = NULL;
	BSTR fwBstrProcessImageFileName = NULL;
	INetFwAuthorizedApplication* fwApp = NULL;
	INetFwAuthorizedApplications* fwApps = NULL;

	do
	{
		BOOL fwAppEnabled = FALSE;
		hr = WindowsFirewallAppIsEnabled(fwProfile,fwProcessImageFileName,&fwAppEnabled);
		if (FAILED(hr))
			break;

		if (fwAppEnabled)
			return S_OK;

		hr = fwProfile->get_AuthorizedApplications(&fwApps);
		if (FAILED(hr))
			break;

		hr = CoCreateInstance(__uuidof(NetFwAuthorizedApplication),NULL,CLSCTX_INPROC_SERVER,__uuidof(INetFwAuthorizedApplication),(void**)&fwApp);
		if (FAILED(hr))
			break;

		fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
		if (fwBstrProcessImageFileName == NULL)
		{
			hr = E_OUTOFMEMORY;
			break;
		}

		hr = fwApp->put_ProcessImageFileName(fwBstrProcessImageFileName);
		if (FAILED(hr))
			break;

		fwBstrName = SysAllocString(fwName);
		if (SysStringLen(fwBstrName) == 0)
		{
			hr = E_OUTOFMEMORY;
			break;
		}

		hr = fwApp->put_Name(fwBstrName);
		if (FAILED(hr))
			break;

		hr = fwApps->Add(fwApp);
		if (FAILED(hr))
			break;

	}while(0);

	if(fwBstrName)
		SysFreeString(fwBstrName);

	if(fwBstrProcessImageFileName)
		SysFreeString(fwBstrProcessImageFileName);

	if (fwApp != NULL)
		fwApp->Release();

	if (fwApps != NULL)
		fwApps->Release();

	return hr;
}

HRESULT CWinFirewallController::WindowsFirewallAppIsEnabled(IN INetFwProfile* fwProfile,IN const wchar_t* fwProcessImageFileName,OUT BOOL* fwAppEnabled)
{
	_ASSERT(fwProfile != NULL);
	_ASSERT(fwProcessImageFileName != NULL);
	_ASSERT(fwAppEnabled != NULL);

	*fwAppEnabled = FALSE;

	HRESULT hr = S_OK;
	BSTR fwBstrProcessImageFileName = NULL;
	INetFwAuthorizedApplication* fwApp = NULL;
	INetFwAuthorizedApplications* fwApps = NULL;

	do
	{
		hr = fwProfile->get_AuthorizedApplications(&fwApps);
		if (FAILED(hr))
			break;

		fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
		if (fwBstrProcessImageFileName == NULL)
		{
			hr = E_OUTOFMEMORY;
			break;
		}

		hr = fwApps->Item(fwBstrProcessImageFileName, &fwApp);
		if (SUCCEEDED(hr))
		{
			VARIANT_BOOL fwEnabled = VARIANT_FALSE;
			hr = fwApp->get_Enabled(&fwEnabled);
			if (FAILED(hr))
				break;

			if (fwEnabled != VARIANT_FALSE)
				*fwAppEnabled = TRUE;
		}
		else
			hr = S_OK;	//这里失败，是改软件还未在列表中

	}while(0);

	if(fwBstrProcessImageFileName)
		SysFreeString(fwBstrProcessImageFileName);

	if (fwApp != NULL)
		fwApp->Release();

	if (fwApps != NULL)
		fwApps->Release();

	return hr;
}

void CWinFirewallController::WindowsFirewallCleanup(IN INetFwProfile* fwProfile)
{
	if (fwProfile != NULL)
		fwProfile->Release();
}

HRESULT CWinFirewallController::WindowsFirewallAddPort(IN INetFwProfile* fwProfile,IN LONG portNumber,IN NET_FW_IP_PROTOCOL ipProtocol,IN const wchar_t* name)
{
	_ASSERT(fwProfile != NULL);
	_ASSERT(name != NULL);

	HRESULT hr = S_OK;
	BSTR fwBstrName = NULL;
	INetFwOpenPort* fwOpenPort = NULL;
	INetFwOpenPorts* fwOpenPorts = NULL;

	do
	{
		BOOL fwPortEnabled = FALSE;
		hr = WindowsFirewallPortIsEnabled(fwProfile,portNumber,ipProtocol,&fwPortEnabled);
		if (FAILED(hr))
			break;

		if (fwPortEnabled)
			return S_OK;

		hr = fwProfile->get_GloballyOpenPorts(&fwOpenPorts);
		if (FAILED(hr))
			break;

		hr = CoCreateInstance(__uuidof(NetFwOpenPort),NULL,CLSCTX_INPROC_SERVER,__uuidof(INetFwOpenPort),(void**)&fwOpenPort);
		if (FAILED(hr))
			break;

		hr = fwOpenPort->put_Port(portNumber);
		if (FAILED(hr))
			break;

		hr = fwOpenPort->put_Protocol(ipProtocol);
		if (FAILED(hr))
			break;

		fwBstrName = SysAllocString(name);
		if (SysStringLen(fwBstrName) == 0)
		{
			hr = E_OUTOFMEMORY;
			break;
		}

		hr = fwOpenPort->put_Name(fwBstrName);
		if (FAILED(hr))
			break;

		hr = fwOpenPorts->Add(fwOpenPort);
		if (FAILED(hr))
			break;

	}while(0);

	if (SysStringLen(fwBstrName) != 0)
		SysFreeString(fwBstrName);

	if (fwOpenPort != NULL)
		fwOpenPort->Release();

	if (fwOpenPorts != NULL)
		fwOpenPorts->Release();

	return hr;
}

HRESULT CWinFirewallController::WindowsFirewallPortIsEnabled(IN INetFwProfile* fwProfile,IN LONG portNumber,IN NET_FW_IP_PROTOCOL ipProtocol,OUT BOOL* fwAppEnabled)
{
	_ASSERT(fwProfile != NULL);
	_ASSERT(fwAppEnabled != NULL);

	*fwAppEnabled = FALSE;

	HRESULT hr = S_OK;
	INetFwOpenPort* fwPort = NULL;
	INetFwOpenPorts* fwPorts = NULL;

	do
	{
		hr = fwProfile->get_GloballyOpenPorts(&fwPorts);
		if (FAILED(hr))
			break;

		hr = fwPorts->Item(portNumber,ipProtocol,&fwPort);
		if (SUCCEEDED(hr))
		{
			VARIANT_BOOL fwEnabled = VARIANT_FALSE;
			hr = fwPort->get_Enabled(&fwEnabled);
			if (FAILED(hr))
				break;

			if (fwEnabled != VARIANT_FALSE)
				*fwAppEnabled = TRUE;
		}
		else
			hr = S_OK;	//Item失败，可能是还没有这个port的记录
	
	}while(0);

	if (fwPort != NULL)
		fwPort->Release();

	if (fwPorts != NULL)
		fwPorts->Release();

	return hr;
}

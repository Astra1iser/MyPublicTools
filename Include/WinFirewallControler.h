#pragma once
#include <netfw.h>

class CWinFirewallController
{
public:
	CWinFirewallController(void) {}
	~CWinFirewallController(void){}

	BOOL AddApp(CString strAppPath,CString strName);

	BOOL AddPort(UINT nPort,NET_FW_IP_PROTOCOL protocol,CString strName);

protected:

	HRESULT WindowsFirewallInitialize(OUT INetFwProfile** fwProfile);

	HRESULT WindowsFirewallAppIsEnabled(IN INetFwProfile* fwProfile,IN const wchar_t* fwProcessImageFileName,OUT BOOL* fwAppEnabled);

	HRESULT WindowsFirewallAddApp(IN INetFwProfile* fwProfile,IN const wchar_t* fwProcessImageFileName,IN const wchar_t* fwName);


	HRESULT WindowsFirewallPortIsEnabled(IN INetFwProfile* fwProfile,IN LONG portNumber,IN NET_FW_IP_PROTOCOL ipProtocol,OUT BOOL* fwAppEnabled);

	HRESULT WindowsFirewallAddPort(IN INetFwProfile* fwProfile,IN LONG portNumber,IN NET_FW_IP_PROTOCOL ipProtocol,IN const wchar_t* name);

	void WindowsFirewallCleanup(IN INetFwProfile* fwProfile);

};
#ifndef _SYSTEM_HELPER_H_
#define _SYSTEM_HELPER_H_

#include <winternl.h>
#include <OSChecker.h>

typedef NTSTATUS (NTAPI *pfnNtQueryInformationProcess)(
	IN	HANDLE ProcessHandle,
	IN	PROCESSINFOCLASS ProcessInformationClass,
	OUT	PVOID ProcessInformation,
	IN	ULONG ProcessInformationLength,
	OUT	PULONG ReturnLength	OPTIONAL
	);

typedef NTSTATUS (WINAPI *PFNNtQuerySystemInformation)(   
	SYSTEM_INFORMATION_CLASS   SystemInformationClass,    // 定义服务类型号
	PVOID   SystemInformation,                            // 用户存储信息的缓冲区
	ULONG   SystemInformationLength,                    // 缓冲区大小
	PULONG   ReturnLength);                            // 返回信息长度

typedef LONG		NTSTATUS;
typedef ULONG		ACCESS_MASK;
//typedef ULONG		KPRIORITY ;		//sdk17763中KPRIORITY定义为LONG
typedef DWORD		ACCESS_MASK ;
//enum {SystemProcessesAndThreadsInformation = 5, ProcessImageFileName = 27};

//#define NT_SUCCESS(status)          ((NTSTATUS)(status)>=0)	//sdk17763中已定义此宏
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)

typedef struct _VM_COUNTERS
{
	ULONG PeakVirtualSize;
	ULONG VirtualSize;
	ULONG PageFaultCount;
	ULONG PeakWorkingSetSize;
	ULONG WorkingSetSize;
	ULONG QuotaPeakPagedPoolUsage;
	ULONG QuotaPagedPoolUsage;
	ULONG QuotaPeakNonPagedPoolUsage;
	ULONG QuotaNonPagedPoolUsage;
	ULONG PagefileUsage;
	ULONG PeakPagefileUsage;
} VM_COUNTERS, *PVM_COUNTERS;

typedef struct _SYSTEM_PROCESSES
{
	ULONG          NextEntryDelta;          //构成结构序列的偏移量；
	ULONG          ThreadCount;             //线程数目；
	ULONG          Reserved1[6];           
	LARGE_INTEGER  CreateTime;              //创建时间；
	LARGE_INTEGER  UserTime;                //用户模式(Ring 3)的CPU时间；
	LARGE_INTEGER  KernelTime;              //内核模式(Ring 0)的CPU时间；
	UNICODE_STRING ProcessName;             //进程名称；
	KPRIORITY      BasePriority;            //进程优先权；
	ULONG          ProcessId;               //进程标识符；
	ULONG          InheritedFromProcessId; //父进程的标识符；
	ULONG          HandleCount;             //句柄数目；
	ULONG          Reserved2[2];
	VM_COUNTERS    VmCounters;              //虚拟存储器的结构；
	IO_COUNTERS    IoCounters;              //IO计数结构； Windows 2000 only
	//SYSTEM_THREADS Threads[1];              //进程相关线程的结构数组；
}SYSTEM_PROCESSES,*PSYSTEM_PROCESSES;


namespace SystemternlHelper
{
	CString DosPathToRealPath(LPCTSTR pcszSrc);							

	int EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable);	

	BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);

	BOOL GetProcessFullPath(DWORD dwPID, TCHAR* pszFullPath);  

	BOOL GetProcessSession(DWORD dwProcessID,DWORD& dwSessionId);
	BOOL GetProcessSession(HANDLE hProcess,CString &strSession );		//获取进程所在的session
	BOOL GetProcessSession(DWORD dwProcessID,CString &strSession );

	BOOL GetProcessSessionID(HANDLE hProcess,CString &strSessionID);	//S-1-5-21-3942060436-2245087920-1392107244-50736

	BOOL GetProcessCmdLine(const DWORD dwPID,CString &strCmdLine);

	BOOL IsSamePath(LPCTSTR pcszSrc, LPCTSTR pcszDest);
	//64位系统下查找进程，可支持x86进程和x64进程，但暂不支持全路径匹配
	HANDLE Findx64Process(LPCTSTR lpcszName, LPCTSTR lpcszFullName, BOOL bCheckFullName, BOOL bNeedTerminate,BOOL bCheckParent,DWORD dwParentID);
	HANDLE FindProcess(LPCTSTR lpcszName, LPCTSTR lpcszFullName, BOOL bCheckFullName = FALSE, BOOL bNeedTerminate = FALSE,BOOL bCheckParent = FALSE,DWORD dwParentID = 0);

	int FindAllProcesses(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax);
	int FindAllx64Processes(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax);

	int GetCPUsage();

	BOOL IsSystemAccount();	//当前是否在system账户下运行

	void ClearTrayIcon();

	PSECURITY_ATTRIBUTES CreateLowSecurityAttribute(void);			//创建一个任何权限都能打开的SecurityAttribute

	BOOL MYFreeSecurityAttribute(PSECURITY_ATTRIBUTES psa);

	BOOL GetTokenByName(HANDLE &hToken,LPTSTR lpName);				//根据进程名获取进程token

	int GetAllTokensByName(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax);				//根据进程名获取所有同名进程的token

	BOOL GetTokenByPid(HANDLE& hToken, DWORD dwProcessID);			//根据进程pid获取进程token

	BOOL GetTokenByPid_thread(HANDLE& hToken, DWORD dwThreadID);	//根据线程pid获取线程token


	BOOL LaunchProcess(LPWSTR lpCmdLine,LPWSTR lpWorkDir,int& nErrCode);

	BOOL LaunchProcessAsUser(LPWSTR lpCmdLine, int& nErrCode,HANDLE* pHandle = NULL);		//创建一个当前登录用户下的进程

	BOOL LaunchSystemProcessWithUserEnv(LPWSTR lpCmdLine);

	BOOL LUAIsElevatedToken(HANDLE hToken, PBOOL pbElevated);

	BOOL LUAGetElevatedToken(HANDLE hToken, PHANDLE phNewToken, PBOOL pbElevated = NULL);

	BOOL CreateProcessByToken(HANDLE hToken, HANDLE hEnvToken, LPCTSTR pszCmd, LPCTSTR pszCwd);

	BOOL CreateProcessAsSystemBySession(LPCTSTR pszCmd, LPCTSTR pszCwd, DWORD dwSession, HANDLE hEnvToken);

	BOOL IsAdminToken(HANDLE hTokenThis);

	BOOL IsLogonUserAdmin();

	BOOL IsUserLogon();

	HANDLE GetProcessFullName(LPCTSTR lpcszName, CString& strFullName);
	HANDLE Get64ProcessFullName(LPCTSTR lpcszName, CString& strFullName);

	HANDLE Get64ProcessFullNameLst(LPCTSTR lpcszName, list<CString>& strFullNameLst);
	HANDLE GetProcessFullNameList(LPCTSTR lpcszName, list<CString>& strFullNameLst);
	int GetAllTokensByNameAndPId(LPCTSTR lpcszName, HANDLE* pTokens, int nMax, DWORD dwPid);
};
#endif
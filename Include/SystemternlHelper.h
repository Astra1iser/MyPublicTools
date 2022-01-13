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
	SYSTEM_INFORMATION_CLASS   SystemInformationClass,    // ����������ͺ�
	PVOID   SystemInformation,                            // �û��洢��Ϣ�Ļ�����
	ULONG   SystemInformationLength,                    // ��������С
	PULONG   ReturnLength);                            // ������Ϣ����

typedef LONG		NTSTATUS;
typedef ULONG		ACCESS_MASK;
//typedef ULONG		KPRIORITY ;		//sdk17763��KPRIORITY����ΪLONG
typedef DWORD		ACCESS_MASK ;
//enum {SystemProcessesAndThreadsInformation = 5, ProcessImageFileName = 27};

//#define NT_SUCCESS(status)          ((NTSTATUS)(status)>=0)	//sdk17763���Ѷ���˺�
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
	ULONG          NextEntryDelta;          //���ɽṹ���е�ƫ������
	ULONG          ThreadCount;             //�߳���Ŀ��
	ULONG          Reserved1[6];           
	LARGE_INTEGER  CreateTime;              //����ʱ�䣻
	LARGE_INTEGER  UserTime;                //�û�ģʽ(Ring 3)��CPUʱ�䣻
	LARGE_INTEGER  KernelTime;              //�ں�ģʽ(Ring 0)��CPUʱ�䣻
	UNICODE_STRING ProcessName;             //�������ƣ�
	KPRIORITY      BasePriority;            //��������Ȩ��
	ULONG          ProcessId;               //���̱�ʶ����
	ULONG          InheritedFromProcessId; //�����̵ı�ʶ����
	ULONG          HandleCount;             //�����Ŀ��
	ULONG          Reserved2[2];
	VM_COUNTERS    VmCounters;              //����洢���Ľṹ��
	IO_COUNTERS    IoCounters;              //IO�����ṹ�� Windows 2000 only
	//SYSTEM_THREADS Threads[1];              //��������̵߳Ľṹ���飻
}SYSTEM_PROCESSES,*PSYSTEM_PROCESSES;


namespace SystemternlHelper
{
	CString DosPathToRealPath(LPCTSTR pcszSrc);							

	int EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable);	

	BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);

	BOOL GetProcessFullPath(DWORD dwPID, TCHAR* pszFullPath);  

	BOOL GetProcessSession(DWORD dwProcessID,DWORD& dwSessionId);
	BOOL GetProcessSession(HANDLE hProcess,CString &strSession );		//��ȡ�������ڵ�session
	BOOL GetProcessSession(DWORD dwProcessID,CString &strSession );

	BOOL GetProcessSessionID(HANDLE hProcess,CString &strSessionID);	//S-1-5-21-3942060436-2245087920-1392107244-50736

	BOOL GetProcessCmdLine(const DWORD dwPID,CString &strCmdLine);

	BOOL IsSamePath(LPCTSTR pcszSrc, LPCTSTR pcszDest);
	//64λϵͳ�²��ҽ��̣���֧��x86���̺�x64���̣����ݲ�֧��ȫ·��ƥ��
	HANDLE Findx64Process(LPCTSTR lpcszName, LPCTSTR lpcszFullName, BOOL bCheckFullName, BOOL bNeedTerminate,BOOL bCheckParent,DWORD dwParentID);
	HANDLE FindProcess(LPCTSTR lpcszName, LPCTSTR lpcszFullName, BOOL bCheckFullName = FALSE, BOOL bNeedTerminate = FALSE,BOOL bCheckParent = FALSE,DWORD dwParentID = 0);

	int FindAllProcesses(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax);
	int FindAllx64Processes(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax);

	int GetCPUsage();

	BOOL IsSystemAccount();	//��ǰ�Ƿ���system�˻�������

	void ClearTrayIcon();

	PSECURITY_ATTRIBUTES CreateLowSecurityAttribute(void);			//����һ���κ�Ȩ�޶��ܴ򿪵�SecurityAttribute

	BOOL MYFreeSecurityAttribute(PSECURITY_ATTRIBUTES psa);

	BOOL GetTokenByName(HANDLE &hToken,LPTSTR lpName);				//���ݽ�������ȡ����token

	int GetAllTokensByName(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax);				//���ݽ�������ȡ����ͬ�����̵�token

	BOOL GetTokenByPid(HANDLE& hToken, DWORD dwProcessID);			//���ݽ���pid��ȡ����token

	BOOL GetTokenByPid_thread(HANDLE& hToken, DWORD dwThreadID);	//�����߳�pid��ȡ�߳�token


	BOOL LaunchProcess(LPWSTR lpCmdLine,LPWSTR lpWorkDir,int& nErrCode);

	BOOL LaunchProcessAsUser(LPWSTR lpCmdLine, int& nErrCode,HANDLE* pHandle = NULL);		//����һ����ǰ��¼�û��µĽ���

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
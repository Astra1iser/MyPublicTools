#pragma once

/*
	���ʹ�ù����ڴ���
	������������� �봴����ͬ���� ���� �������Ӷ˳�ʼ��CreateSHareMemory()����,���Ӷ˳�ʼ��OpenSharedMemory() �����ظ���ʼ��
	��ʼ���󼴿�ʹ��SetSharedMemory()��GetSharedMemory()��ȡ����,ע��,��ȡ������ԭ�Ӳ����Ҳ����������ָ��

	SharedMemory* abc = new SharedMemory(L"ShipMem");
	abc->CreateSharedMemory(sizeof(string),L"ServerWriteOver", L"ClientReadOver");
	string i;
	do
	{
		cin >> i;
		SetEvent(abc->hClientReadOver);
		abc->SetSharedMemory(i);

	} while (true);





	SharedMemory* abc = new SharedMemory(L"ShipMem");
	abc->OpenSharedMemory(L"ServerWriteOver", L"ClientReadOver");
	int i;
	do {
		abc->GetSharedMemory(i);
		cout << i << endl;
	} while (1);






*/

class SharedMemory
{
private:
	HANDLE hShipFileMappingCreator;
	HANDLE hShipFileMappingVisitor;
	//LPVOID lpShipMemCreator;
	//LPVOID lpShipMemVisitor;
	//HANDLE hServerWriteOver;
	//HANDLE hClientReadOver;
	CString SharedMemoryName;
	//CString ServerWriteOverName;
	//CString ClientReadOverName;

public:
	LPVOID lpShipMemCreator;
	LPVOID lpShipMemVisitor;
	HANDLE hServerWriteOver;
	HANDLE hClientReadOver;

	SharedMemory(CString SharedMemoryName);

	BOOL CreateSharedMemory(unsigned long DataSize, CString ServerWriteOverName, CString ClientReadOverName);

	BOOL OpenSharedMemory(CString ServerWriteOverName, CString ClientReadOverName);

	template<class T>
	void SetSharedMemory(_In_ const T& Data)
	{
		WaitForSingleObject(hClientReadOver, INFINITE);
		T* lp = (T*)lpShipMemCreator;
		//*lp = Data;
		memcpy(lp, &Data, sizeof(Data));
		ResetEvent(hClientReadOver);
		SetEvent(hServerWriteOver);
	}

	template<class T>
	void GetSharedMemory(_Out_ T& Data)
	{
		SetEvent(hClientReadOver);
		WaitForSingleObject(hServerWriteOver, INFINITE);
		T* lp = (T*)lpShipMemCreator;
		Data = *lp;
		ResetEvent(hServerWriteOver);

	}

	//����Ԥ��Ҫʵ��һ������ӳ���С�ĺ���
	void ReSetSharedMemory()
	{

	}

	~SharedMemory();
};


#pragma once

/*
	如何使用共享内存类
	如果是两个进程 请创建相同的类 其中 建立链接端初始化CreateSHareMemory()函数,连接端初始化OpenSharedMemory() 请勿重复初始化
	初始化后即可使用SetSharedMemory()和GetSharedMemory()存取数据,注意,存取数据是原子操作且不能往里面放指针

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

	//这里预期要实现一个重置映射大小的函数
	void ReSetSharedMemory()
	{

	}

	~SharedMemory();
};


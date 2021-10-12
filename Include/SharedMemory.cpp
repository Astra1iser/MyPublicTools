#pragma once
#include <SharedMemory.h>

SharedMemory::SharedMemory(CString SharedMemoryName)
{
	hShipFileMappingCreator = NULL;
	hShipFileMappingVisitor = NULL;
	lpShipMemCreator = NULL;
	lpShipMemVisitor = NULL;
	hServerWriteOver = NULL;
	hClientReadOver = NULL;
	this->SharedMemoryName = SharedMemoryName;
}

BOOL SharedMemory::CreateSharedMemory(unsigned long DataSize, CString ServerWriteOverName, CString ClientReadOverName)
{
	while (!hShipFileMappingCreator)
	{
		hShipFileMappingCreator = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, DataSize, SharedMemoryName);
	}

	lpShipMemCreator = MapViewOfFile(hShipFileMappingCreator, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (!lpShipMemCreator)
	{
		cout << "MapView Of File failed : " << GetLastError() << endl;
		return FALSE;
	}
	hServerWriteOver = CreateEvent(NULL, TRUE, FALSE, ServerWriteOverName);
	hClientReadOver = CreateEvent(NULL, TRUE, FALSE, ClientReadOverName);
	if (hServerWriteOver == NULL || hClientReadOver == NULL)
	{
		cout << "CreateEvent : " << GetLastError() << endl;
		return FALSE;
	}
	return TRUE;
}

BOOL SharedMemory::OpenSharedMemory(CString ServerWriteOverName, CString ClientReadOverName)
{
	while (!hShipFileMappingVisitor)
	{
		hShipFileMappingVisitor = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SharedMemoryName);
	}
	if (!hShipFileMappingVisitor)
	{
		cout << "open HLAObject FileMapping failed : " << GetLastError() << endl;
		return FALSE;
	}


	lpShipMemVisitor = MapViewOfFile(hShipFileMappingVisitor, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (!lpShipMemVisitor)
	{
		cout << "MapViewOfFile failed :  " << GetLastError() << endl;
		return FALSE;
	}


	hServerWriteOver = CreateEvent(NULL, TRUE, FALSE, _T("ServerWriteOver"));
	hClientReadOver = CreateEvent(NULL, TRUE, FALSE, _T("ClientReadOver"));
	if (NULL == hServerWriteOver || NULL == hClientReadOver)
	{
		cout << "CreateEvent" << GetLastError() << endl;
		return FALSE;
	}
	return TRUE;
}

SharedMemory::~SharedMemory()
{
	if (hShipFileMappingCreator != NULL)
		CloseHandle(hShipFileMappingCreator);
	if (hShipFileMappingVisitor != NULL)
		CloseHandle(hShipFileMappingVisitor);
	if (hServerWriteOver != NULL)
		CloseHandle(hServerWriteOver);
	if (hClientReadOver != NULL)
		CloseHandle(hClientReadOver);
	if (lpShipMemCreator != NULL)
		delete lpShipMemCreator;
	if (lpShipMemVisitor != NULL)
		delete lpShipMemVisitor;
}
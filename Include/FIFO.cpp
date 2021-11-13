#pragma once
#include <FIFO.h>


HANDLE FIFO::CreateFIFO(CString PipeName)
{
	CString buffer;
	buffer.Format(L"\\\\.\\Pipe\\%s", PipeName.GetBuffer());
	HANDLE h_Pipe = CreateNamedPipe(
		buffer,												//�ܵ�����
		PIPE_ACCESS_DUPLEX,									//�ܵ�����
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	//�ܵ�����
		PIPE_UNLIMITED_INSTANCES,							//�ܵ��ܴ��������ʵ������
		0,													//����������ĳ��� 0��ʾĬ��
		0,													//���뻺�����ĳ��� 0��ʾĬ��
		NMPWAIT_WAIT_FOREVER,								//��ʱʱ��
		NULL);

	if (h_Pipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to CreateNamedPipe:";
		Wcout(PipeName);
		return NULL;
	}
	else
	{
		std::cout << "CreateNamedPipe success!" << std::endl;
		return h_Pipe;
	}
}

HANDLE FIFO::WaitFIFO(CString PipeName)
{
	CString buffer;
	buffer.Format(L"\\\\.\\Pipe\\%s", PipeName.GetBuffer());
	BOOL bRet = WaitNamedPipe(buffer, NMPWAIT_WAIT_FOREVER);

	if (!bRet)
	{
		std::cout << "Failed to connect pipeline!" << std::endl;
		return FALSE;
	}
	else
	{
		std::cout << "Success to connect pipeline!" << std::endl;

		HANDLE h_Pipe = CreateFile(						//�ܵ�����һ��������ļ�
			buffer,										//�ļ�����
			GENERIC_READ | GENERIC_WRITE,				//�ļ�ģʽ
			0,											//�Ƿ���
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,						//�ļ�����(ֻ����Ĭ��...)NORMAL ΪĬ������
			NULL);

		if (h_Pipe == INVALID_HANDLE_VALUE)
		{
			std::cout << "Failed to CreateFile pipeline file!" << std::endl;
			return NULL;
		}
		else
		{
			return h_Pipe;
		}
	}
}

BOOL FIFO::ConnectFIFO(HANDLE h_Pipe)
{
	if (ConnectNamedPipe(h_Pipe, NULL) == NULL)				//�����ͻ���������
	{
		std::cout << "Failed to Connect!" << std::endl;
		return FALSE;
	}
	else
	{
		std::cout << "Connect success!" << std::endl;
		return TRUE;
	}
}

BOOL FIFO::CloseFIFO(HANDLE h_Pipe)
{
	if (CloseHandle(h_Pipe))
	//if (CancelIoEx(h_Pipe, NULL))
	{
		cout << "Colse pipe Success!";
		return TRUE;
	}
	else
	{
		cout << "Close pipe Failed!";
		return FALSE;
	}
}
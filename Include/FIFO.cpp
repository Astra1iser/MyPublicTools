#pragma once
#include <FIFO.h>


HANDLE FIFO::CreateFIFO(CString PipeName)
{
	CString buffer;
	buffer.Format(L"\\\\.\\Pipe\\%s", PipeName.GetBuffer());
	HANDLE h_Pipe = CreateNamedPipe(
		buffer,												//管道名字
		PIPE_ACCESS_DUPLEX,									//管道类型
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	//管道参数
		PIPE_UNLIMITED_INSTANCES,							//管道能创建的最大实例数量
		0,													//输出缓冲区的长度 0表示默认
		0,													//输入缓冲区的长度 0表示默认
		NMPWAIT_WAIT_FOREVER,								//超时时间
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

		HANDLE h_Pipe = CreateFile(						//管道属于一种特殊的文件
			buffer,										//文件名字
			GENERIC_READ | GENERIC_WRITE,				//文件模式
			0,											//是否共享
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,						//文件属性(只读，默认...)NORMAL 为默认属性
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
	if (ConnectNamedPipe(h_Pipe, NULL) == NULL)				//阻塞客户端来连接
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
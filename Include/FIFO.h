#pragma once
/*
	HANDLE h_Pipe = CreateFIFO(L"mypipe");
	ConnectNamedPipe(h_Pipe, NULL);


	如何使用命名管道:
	1.HANDLE h_Pipe = CreateFIFO(L"mypipe");  //首先一个客户端端创建命名管道
	2.WaitFIFO(L"mypipe"); //另一个客户端等待管道信号,这个函数如果执行时没有检测到对应的命名管道,则会直接返回,如果检测到了对应的命名管道,
	则等待发送者使用ConnectFIFO()建立链接并创建管道文件
	3.等待1和2完成时,则两端之间可使用ReadFIFO()和WriteFIFO进行通信
	4.通信完成时请使用CloseFIFO()关闭管道

	注意:命名管道是个原子操作,如果一端进行了管道的写操作,那直到另一端进行读操作或者进行关闭管道之前,本次写操作会一直阻塞
	*/


namespace FIFO
{
	HANDLE CreateFIFO(CString PipeName);

	HANDLE WaitFIFO(CString PipeName);

	BOOL ConnectFIFO(HANDLE h_Pipe);

	BOOL CloseFIFO(HANDLE h_Pipe);

	template<class T>
	DWORD ReadFIFO(HANDLE h_Pipe, T& buffer, DWORD nNumberOfBytesToRead)
	{
		DWORD len = 0;
		//if (ReadFile(h_Pipe, &buffer, sizeof(buffer), &len, NULL) == FALSE)	//接收内容
		if (ReadFile(h_Pipe, &buffer, nNumberOfBytesToRead, &len, NULL) == FALSE)	//接收内容
		{
			std::cout << "Failed to read data!" << std::endl;
			return FALSE;
		}
		else
		{
			std::cout << "Read data Success!" << std::endl;
			return len;
		}
	}

	template<class T>
	DWORD WriteFIFO(HANDLE h_Pipe, T& buffer, DWORD nNumberOfBytesToWrite)
	{
		DWORD len = 0;
		//if (WriteFile(h_Pipe, &buffer, sizeof(buffer), &len, NULL) == FALSE)	//发送内容
		if (WriteFile(h_Pipe, &buffer, nNumberOfBytesToWrite, &len, NULL) == FALSE)	//发送内容
		{
			std::cout << "Failed to write data!" << std::endl;
			return FALSE;
		}
		else
		{
			std::cout << "Write data Success!" << std::endl;
			return len;
		}

	}
}

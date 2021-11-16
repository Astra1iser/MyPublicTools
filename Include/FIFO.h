#pragma once
/*
	HANDLE h_Pipe = CreateFIFO(L"mypipe");
	ConnectNamedPipe(h_Pipe, NULL);


	���ʹ�������ܵ�:
	1.HANDLE h_Pipe = CreateFIFO(L"mypipe");  //����һ���ͻ��˶˴��������ܵ�
	2.WaitFIFO(L"mypipe"); //��һ���ͻ��˵ȴ��ܵ��ź�,����������ִ��ʱû�м�⵽��Ӧ�������ܵ�,���ֱ�ӷ���,�����⵽�˶�Ӧ�������ܵ�,
	��ȴ�������ʹ��ConnectFIFO()�������Ӳ������ܵ��ļ�
	3.�ȴ�1��2���ʱ,������֮���ʹ��ReadFIFO()��WriteFIFO����ͨ��
	4.ͨ�����ʱ��ʹ��CloseFIFO()�رչܵ�

	ע��:�����ܵ��Ǹ�ԭ�Ӳ���,���һ�˽����˹ܵ���д����,��ֱ����һ�˽��ж��������߽��йرչܵ�֮ǰ,����д������һֱ����
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
		//if (ReadFile(h_Pipe, &buffer, sizeof(buffer), &len, NULL) == FALSE)	//��������
		if (ReadFile(h_Pipe, &buffer, nNumberOfBytesToRead, &len, NULL) == FALSE)	//��������
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
		//if (WriteFile(h_Pipe, &buffer, sizeof(buffer), &len, NULL) == FALSE)	//��������
		if (WriteFile(h_Pipe, &buffer, nNumberOfBytesToWrite, &len, NULL) == FALSE)	//��������
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

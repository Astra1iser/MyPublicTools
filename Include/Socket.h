#pragma once

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
const int MAX_BUF_LEN = 255;


class MySocket
{
public:
	MySocket();
	~MySocket();
	BOOL InitSender(u_short port, string ip = "");
	BOOL InitReceiver(u_short port);

	//BOOL Send(string buffer);
	template<class T>
	BOOL Send(T buffer)
	{
		string strbuffer;

		if (typeid(string) == typeid(buffer))
		{
			strbuffer = buffer;
		}
		else if(typeid(int) == typeid(buffer))
		{
			strbuffer = to_string((int)buffer);
		}
		else
		{
			return FALSE;
		}

		int nSize = sizeof(SOCKADDR_IN);
		int nSendSize = sendto(sock_type, strbuffer.c_str(), strlen(strbuffer.c_str()), 0, (SOCKADDR*)&SendUdpServ, nSize);
		if (nSendSize == SOCKET_ERROR)
		{
			err = WSAGetLastError();
			printf("sendto error:%d\n", err);
			//closesocket(sock_type);
			WSACleanup();
			return FALSE;
		}
		else
		{
			printf("send: %s\n", strbuffer.c_str());
			return TRUE;
		}
	}



	string Receive();


	WORD wVersionRequested;
	WSADATA wsaData;//指向WinSocket信息结构的指针
	int err;
	SOCKET sock_type;


	SOCKADDR_IN SendUdpServ;
	SOCKADDR_IN ReceiveUdpServ;
};
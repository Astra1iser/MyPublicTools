/*
如何使用Mysocket进行通讯
MySocket socket;
接收者必须使用socket.InitReceiver(端口号);来指定本地接收端口
发送者必须使用socket.InitSender(端口号,接收者IP不填则默认本次是本地通讯)
InitReceiver和InitSender可多次重新指向

发送端可使用socket.Send();来发送string或者int类型数据
接收端可使用socket.Receive();来接收数据 注意Receive()方法在未建立socket连接时会发生阻塞


本类具体例子:

MySocket aaaaaaa;

class Example3 : public Thread
{
public:
	//重写Run;

	void Run() override
	{
		aaaaaaa.InitSender(7001);
		int nLoop = 0;
		char buff[MAX_BUF_LEN];
		memset(buff, 0, MAX_BUF_LEN);

		while (this->bRun)
		{
			nLoop++;

			//sprintf(buff, "Message %d", nLoop);
			//aaaaaaa.Send(buff);
			aaaaaaa.Send(nLoop);
			Sleep(5000);
		}
		return;
	}

};

class Example4 : public Thread
{
public:
	//重写Run;

	void Run() override
	{
		aaaaaaa.InitReceiver(7001);

		while (this->bRun)
		{
			aaaaaaa.Receive();
		}
		return;
	}

};

int main()
{
	Example3 ddd;
	Example4 dddd;
	ddd.Start();
	dddd.Start();

	system("pause");


	return 0;
}

*/



#pragma once
#include <Socket.h>


MySocket::MySocket()
{
	wVersionRequested = MAKEWORD(2, 2);
	// 初始化winsock库，使用socket的前提
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return;
	}
	sock_type = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_type == INVALID_SOCKET)
	{
		err = WSAGetLastError();
		printf("socket error:%d\n", err);
		closesocket(sock_type);
		WSACleanup();
		return;
	}
}

MySocket::~MySocket()
{
	closesocket(sock_type);
	WSACleanup();
}

BOOL MySocket::InitSender(u_short port, string ip)
{
	BOOL bOpt;
	bOpt = TRUE;
	//打开广播选项
	setsockopt(sock_type, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));

	// 设置发往的地址
	memset(&SendUdpServ, 0, sizeof(SendUdpServ));
	SendUdpServ.sin_family = AF_INET;
	if ("" == ip)
	{
		SendUdpServ.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
	}
	else
	{
		SendUdpServ.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	}

	SendUdpServ.sin_port = htons(port);

	return TRUE;
}


//BOOL MySocket::Send(string buffer)
//{
//	int nSize = sizeof(SOCKADDR_IN);
//	int nSendSize = sendto(conn_sock, buffer.c_str(), strlen(buffer.c_str()), 0, (SOCKADDR*)&SendUdpServ, nSize);
//	if (nSendSize == SOCKET_ERROR)
//	{
//		err = WSAGetLastError();
//		printf("sendto error:%d\n", err);
//		closesocket(conn_sock);
//		WSACleanup();
//		return 0;
//	}
//	else
//	{
//		printf("send: %s\n", buffer.c_str());
//		return TRUE;
//	}
//}

BOOL MySocket::InitReceiver(u_short port)
{
	//创建本地地址信息并绑定

	ReceiveUdpServ.sin_family = AF_INET;
	ReceiveUdpServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ReceiveUdpServ.sin_port = htons(port);
	if (bind(sock_type, (SOCKADDR*)&ReceiveUdpServ, sizeof(ReceiveUdpServ)) != 0)
	{
		err = WSAGetLastError();
		printf("bind error:%d\n", err);
		closesocket(sock_type);
		WSACleanup();
		return 0;
	}
	else
		return TRUE;
}


string MySocket::Receive()
{
	int addrLen = sizeof(SOCKADDR_IN);
	char recvBuffer[1024];
	memset(recvBuffer, 0, 1024);

	//while (TRUE)
	//{
		int nSize = recvfrom(sock_type, recvBuffer, 1024, 0, (SOCKADDR*)&ReceiveUdpServ, &addrLen);
		if (nSize == SOCKET_ERROR)
		{
			err = WSAGetLastError();
			printf("recv error:%d\n", err);
			return "";
		}
		else
		{
			//recvBuffer[nSize] = '\0';
			printf("receive: %s\n", recvBuffer);
			return recvBuffer;
		}
	//}
}
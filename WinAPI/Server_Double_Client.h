#pragma once

#include "WinApi_Shablon.h"
#include <iostream>

extern DWORD WINAPI ThreadProc(LPVOID lpParameter);

class ThreadConverter
{
	friend DWORD WINAPI ThreadProc(LPVOID lpParameter); // пояснения см. ниже
	HANDLE m_hThread[2];

public:
	DWORD Terminate(BOOL bCritical = FALSE);
	BOOL Execute();
	ThreadConverter();
	virtual ~ThreadConverter();

protected:
	int it = 0;
	DWORD m_dwID[2];
	virtual DWORD ThreadFunc() = 0;
	BOOL m_bTerminated;
};

class Client : private ThreadConverter
{

public:

	enum Packet {
		P_ChatMessage,
		P_Test
	};
	void Start(std::string, int);
	

private:

	DWORD ThreadFunc();
	


protected:

	SOCKET Connection;
	int Counter = 0;
	WSAData wsaData;
	WORD DLLVersion;
	SOCKADDR_IN addr;
	SOCKET sListen;
	SOCKET newConnection;
	Packet msgtype;

};
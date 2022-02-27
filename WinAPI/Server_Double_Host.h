#pragma once
#include "WinApi_Shablon.h"


extern DWORD WINAPI ThreadProc1(LPVOID lpParameter);

class ThreadConverterH
{
	friend DWORD WINAPI ThreadProc(LPVOID lpParameter); // пояснения см. ниже
	std::vector<HANDLE> m_hThread;

public:
	DWORD Terminate(BOOL bCritical = FALSE);
	BOOL Execute(int n);
	ThreadConverterH();
	virtual ~ThreadConverterH();

public:
	std::vector<bool> pool_index;
	std::vector<DWORD> m_dwID;
	virtual DWORD ThreadFunc(int n) = 0;
	std::vector<BOOL> m_bTerminated;
};

class Server : private ThreadConverterH
{
public:



	void Start(std::string, int);

private:

	DWORD ThreadFunc(int index);

	void Find();


protected:

	std::vector< SOCKET> Connections;
	int Counter = 0;
	WSAData wsaData;
	WORD DLLVersion;
	SOCKADDR_IN addr;
	SOCKET sListen;
	SOCKET newConnection;
	int sizeofaddr;



};

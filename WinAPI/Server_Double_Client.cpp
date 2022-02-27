#include "Server_Double_Client.h"
#include "Bridge_Interface_to_Server.h"
enum switch_mode
{
	Input,
	Output
};
switch_mode Mode;

extern DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	ThreadConverter* pMyThread = (ThreadConverter*)lpParameter;
	return pMyThread->ThreadFunc();
}

ThreadConverter::ThreadConverter()
{
	m_bTerminated = FALSE;
	m_hThread[0] = NULL;
	m_hThread[1] = NULL;
}
ThreadConverter::~ThreadConverter()
{
	Terminate(FALSE);
}
BOOL ThreadConverter::Execute()
{
	if (m_hThread[it])
	{
		return FALSE;
	}
	m_hThread[it] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, &m_dwID[it]);
	it++;
	return (m_hThread == NULL) ? FALSE : TRUE;

}
DWORD ThreadConverter::Terminate(BOOL bCritical)
{
	if (m_hThread == NULL)
		return (DWORD)-1;

	m_bTerminated = TRUE;
	DWORD dwExitCode;

	if (!bCritical)
	{
		for (int i = 10; i > 0; i--)
		{
			GetExitCodeThread(m_hThread[0], &dwExitCode);
			GetExitCodeThread(m_hThread[1], &dwExitCode);
			if (dwExitCode == STILL_ACTIVE)
				Sleep(25);
			else
				break;
		}
	}

	GetExitCodeThread(m_hThread, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)
		TerminateThread(m_hThread[0], -1);
		TerminateThread(m_hThread[1], -1);
	GetExitCodeThread(m_hThread[0], &dwExitCode);
	GetExitCodeThread(m_hThread[1], &dwExitCode);
	CloseHandle(m_hThread[0]);
	CloseHandle(m_hThread[1]);
	m_hThread[0] = NULL;
	m_hThread[1] = NULL;
	m_bTerminated = FALSE;
	return dwExitCode;
}

void Client::Start(std::string IP, int PORT)
{

		DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr(IP.c_str());
		addr.sin_port = htons(PORT);
		addr.sin_family = AF_INET;

		Connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			Check_Signals::is_conected_with_server = false;
		} else Check_Signals::is_conected_with_server = true;
		std::cout << "Connected!\n";



		Mode = switch_mode::Input;

		Execute();

		Mode = switch_mode::Output;

		Execute();

}

DWORD Client::ThreadFunc() {
	switch (Mode)
	{
		
	case Input:
	{

		while (true) {
	
			Mode = switch_mode::Output;
			ServerData data;
			if (recv(Connection, (char*)&data, sizeof(ServerData), NULL) <= 0) {
				Sleep(2000); exit(0);
			}

			Bridge_IS::Bridge_StI(data);

			if (m_bTerminated) break;
		}

		break;
	}
	case Output:
	{

		while (true)
		{	
			
			Mode = switch_mode::Input;
			if (Bridge_IS::S_OUT.size())
			{
				if (send(Connection, (char*)&Bridge_IS::S_OUT[0], sizeof(ServerData), 0) <= 0) {
						Sleep(2000); exit(0);
				}
				if(Bridge_IS::S_OUT.size() > 0) Bridge_IS::S_OUT.pop_front();
			}
			Sleep(1);
			if (m_bTerminated) break;
		}

		break;
	}

	}
		
		closesocket(Connection);
		return 0;
}






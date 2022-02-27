#include "Server_Double_Host.h"
#include <thread>
int index1;

extern DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{
	ThreadConverterH* pMyThread = (ThreadConverterH*)lpParameter;
	return pMyThread->ThreadFunc(index1);
}

ThreadConverterH::ThreadConverterH()
{}

ThreadConverterH::~ThreadConverterH()
{
	Terminate(FALSE); 
}

BOOL ThreadConverterH::Execute(int n)
{
	index1 = n;

	pool_index.push_back(true);
	m_hThread.push_back(HANDLE());
	m_dwID.push_back(DWORD());
	m_bTerminated.push_back(BOOL());


	m_hThread[index1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1, this, 0, &m_dwID[index1]);

	return (m_hThread[index1] == NULL) ? FALSE : TRUE;

}

DWORD ThreadConverterH::Terminate(BOOL bCritical)
{
	
	for (int i = 0; i < pool_index.size(); i++)
	{
		if (pool_index[i])
		{
			m_bTerminated[i] = TRUE;

			DWORD dwExitCode;

			if (!bCritical)
			{
				for (int i = 10; i > 0; i--)
				{
					GetExitCodeThread(m_hThread[i], &dwExitCode);
					if (dwExitCode == STILL_ACTIVE)
						Sleep(25);
					else
						break;
				}
			}

			GetExitCodeThread(m_hThread[i], &dwExitCode);
			if (dwExitCode == STILL_ACTIVE)
				TerminateThread(m_hThread[i], -1);
			GetExitCodeThread(m_hThread[i], &dwExitCode);
			CloseHandle(m_hThread[i]);
			m_hThread[i] = NULL;
			m_bTerminated[i] = FALSE;
			return dwExitCode;
		}
	}
}

	enum Packet {
		P_ChatMessage,
		P_Test
	};

	void Server::Start(std::string IP, int PORT)
	{

		DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			//std::cout << "Error with Socket Version" << std::endl;
			exit(1);
		}

		sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr(IP.c_str());
		addr.sin_port = htons(PORT);
		addr.sin_family = AF_INET;

		sListen = socket(AF_INET, SOCK_STREAM, NULL);
		bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
		listen(sListen, SOMAXCONN);


		std::thread find(&Server::Find, this);
		find.detach();

	}

	void Server::Find()
	{
		for (int i = 0; i < 100; i++) {
			newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

			if (newConnection == 0) {
				//std::cout << "Error with Connection to addr\n";
				i--;
			}
			else {
				//std::cout << "Client Connected!\n";

				Execute(i);

				Connections.push_back(newConnection);


			}
		}
	}

	DWORD Server::ThreadFunc (int index) {
		while (true) {
			ServerData packet;
			if (recv(Connections[index], (char*)&packet, sizeof(ServerData), NULL) <= 0) { break; };

			//std::cout << "INPUT: " << index<<'\n';
			//std::wcout << packet.msg << " "<<'\n';

			for (int i=0;i<Connections.size();i++)
			{
				if (pool_index[i] == true)
				{
					if (i != index)
					{

						//std::cout << "OUTPUT: " << i << '\n';

						send(Connections[i], (char*)&packet, sizeof(ServerData), NULL);
					}
				}
			}

			if (m_bTerminated[index]) break;

		}
		pool_index[index] = false;
		closesocket(Connections[index]);
		return 0;
	}






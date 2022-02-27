#include "WinApi_Shablon.h"
#include "Server_Double_Client.h"
#include "Bridge_Interface_to_Server.h"
#include "Server_Double_Host.h"

int CALLBACK wWinMain ( HINSTANCE, HINSTANCE, PWSTR, int)
{
	Client client;
	CApp app{}; 
	Server server{};

	Parser Meta("KEY_DOUBLE.txt");
	try
	{
		Meta.Start();
	}
	catch (const std::exception& ex)
	{
		std::string expt_data = ex.what();

		MessageBox(nullptr, std::wstring(begin(expt_data), end(expt_data)).c_str(), L"Error", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
		exit(0);
	}

	if (Meta.HOST)
	{
		try 
		{
			server.Start(Meta.IP,Meta.PORT);
		}
		catch (...)
		{
			MessageBox(nullptr, L"Sorry,Erorr_with_Host", L"Error", MB_ICONERROR | MB_OK);
			Sleep(1000);
			exit(0);
		}
	}

	Sleep(500);
	Bridge_IS::key = Meta.KEY;

	client.Start(Meta.IP , Meta.PORT);

	app.nickname = Meta.NICKNAME;

	return app.Run();
}
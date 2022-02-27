#include "WinApi_Shablon.h"


void CApp::Input_Port()
{
	while (1) {

		Check_Signals::is_conected_with_client = true;

		Sleep(10);
		if (Bridge_IS::S_IN.size() > 40) {
			Check_Signals::is_conected_with_client = false;
			MessageBox(m_hWnd, L"DDOS_ATACK_BLOCKED", L"PLEASE_WAIT", MB_ICONINFORMATION);
			int k = 0;
			while (Bridge_IS::S_IN.size() > 20)
			{
				k++;
				Bridge_IS::S_IN.clear();
				Sleep(1000);
				if (k == 10) {
					exit(0);
					MessageBox(m_hWnd, L"CRITICAL_EXIT", L"SECURITY", MB_ICONINFORMATION);
				}
			}
			
		}

		if (Bridge_IS::S_IN.size() > 0)
		{
			
			Chat_Msg.print(Bridge_IS::conv_ServData_to_msg(Bridge_IS::S_IN[0].nickname).c_str(),
								Bridge_IS::conv_ServData_to_msg(Bridge_IS::S_IN[0].msg).c_str());
			
			Sleep(100); // Antispam

			Bridge_IS::S_IN.pop_front();
		}
	}
}
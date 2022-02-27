#include "WinApi_Shablon.h"

#define CASE(a) }return 0;case(a):{
#define DEFAULT break;default:
#define END }return 0;
#define BEGIN {


LRESULT CApp::window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		BEGIN
		
		CASE (WM_COMMAND)

			switch (static_cast<CApp::CTL_ID>(LOWORD(wParam)))
			{
				BEGIN

				CASE(CApp::CTL_ID::Send_Msg)

					Send_Msg(hWnd);
				
				    
				END
			}

		CASE(WM_KEYDOWN)

			switch (wParam)
			{
				BEGIN

				CASE(VK_RETURN)

					Send_Msg(hWnd);

				END
			}

		CASE(WM_CTLCOLOREDIT)

			if ((HWND)lParam == GetDlgItem(hWnd, static_cast<int>(CApp::CTL_ID::Edit_Text)))
			{
			SetBkColor((HDC)wParam, RGB(50, 50, 50));
			SetTextColor((HDC)wParam, RGB(240, 240, 240));
			return (LRESULT)GetSysColorBrush(RGB(40, 40, 40));
			}
			
		CASE (WM_DRAWITEM)
			
				static HFONT hfontButton = CreateFont(16, 0, 0, 0, 600, 0, 0, 0, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH, L"Roboto");
				LPDRAWITEMSTRUCT Item = (LPDRAWITEMSTRUCT)lParam;

				/* установить отображение текста */
				SetBkMode(Item->hDC, TRANSPARENT);
				SetTextColor(Item->hDC, 0xFFFFFF);
				SelectObject(Item->hDC, hfontButton);

				/* белый фон при нажатии */
				if (Item->itemState & ODS_SELECTED)
					FillRect(Item->hDC, &Item->rcItem, (HBRUSH)(RGB(50,50,50)));
				else
					FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(GRAY_BRUSH));

				/* нарисовать текст */
				int len = GetWindowTextLength(Item->hwndItem);
				char* buf = new char[len + 1];
				GetWindowTextA(Item->hwndItem, buf, len + 1);
				DrawTextA(Item->hDC, buf, len, &Item->rcItem, DT_CENTER);

				return true;
			
		
		CASE(WM_DESTROY)


			PostQuitMessage(EXIT_SUCCESS);
			exit(0);

		END
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};

void CApp::Send_Msg(HWND& hWnd)
{
	const int length = GetWindowTextLength(this->m_hWndTextArea_Msg);
	wchar_t* text = new wchar_t[length + 1];

	try {

		GetWindowText(this->m_hWndTextArea_Msg, text, length+1);
		SetWindowText(this->m_hWndTextArea_Msg, 0);
		
		text[length] = '\0';

		if (text[0] == '\0') throw std::exception("Sorry, empty Message");
		if (length > Max_Message_Size) throw std::exception("Sorry, very Big Message");

	}
	catch (std::exception& ex) {
		std::string str = ex.what(); MessageBox(
			hWnd, std::wstring(begin(str), end(str)).c_str(), L"Error", MB_ICONINFORMATION);

		delete text;
		text = nullptr;

		return;
	}
	catch (...)
	{

		delete text;
		text = nullptr;

		exit(0);
	}
	
	ServerData msg;

	Bridge_IS::conv_msg_to_ServData(msg.nickname, nickname.c_str());

	Bridge_IS::conv_msg_to_ServData(msg.msg, text);
	
	Bridge_IS::Bridge_ItS(msg);

	Chat_Msg.print(nickname.c_str(),text,TRUE);

	delete text;
	text = nullptr;
}

LRESULT CApp::application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CApp* pApp;
	if (uMsg == WM_NCCREATE)
	{
		pApp = static_cast<CApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp)))
		{
			if (GetLastError() != 0)
				return false;
		}
	}
	else
	{
		pApp = reinterpret_cast<CApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pApp)
	{
		pApp->m_hWnd.object = hWnd;
		return pApp->window_proc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};
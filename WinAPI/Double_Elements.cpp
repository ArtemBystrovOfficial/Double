#include "WinApi_Shablon.h"
#include <fstream>

void CApp::create_native_controls()
{


	
	// BUTTON
	if (!(this->m_hWndButton_Send_Msg.object = CreateWindowEx(0,
		m_hWndButton_Send_Msg.o_class.c_str(),
		m_hWndButton_Send_Msg.o_name.c_str(),
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_OWNERDRAW,
		m_hWndButton_Send_Msg.x,
		m_hWndButton_Send_Msg.y,
		m_hWndButton_Send_Msg.width,
		m_hWndButton_Send_Msg.height, this->m_hWnd.object,
		reinterpret_cast<HMENU>(CApp::CTL_ID::Send_Msg),
		nullptr, nullptr))) 
	throw std::exception("Button_Error");

	

	// TEXT AREA
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


	if (!(this->m_hWndTextArea_Msg.object = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		m_hWndTextArea_Msg.o_class.c_str(),
		m_hWndTextArea_Msg.o_name.c_str(),
		WS_CHILD  | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL,
		m_hWndTextArea_Msg.x,
		m_hWndTextArea_Msg.y,
		m_hWndTextArea_Msg.width,
		m_hWndTextArea_Msg.height, this->m_hWnd.object,
		reinterpret_cast<HMENU>(CApp::CTL_ID::Edit_Text),
		nullptr, nullptr)))
		throw std::exception("Text_Area");

	ShowScrollBar(this->m_hWndTextArea_Msg, SB_VERT,false);

	SendMessage(this->m_hWndTextArea_Msg.object, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndButton_Send_Msg.object, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	HBRUSH color((HBRUSH)RGB(40, 40, 20));

	MENUINFO mi = { 0 };
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_BACKGROUND | MIM_APPLYTOSUBMENUS;
	mi.hbrBack = color;
	mi.hbrBack = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	HMENU hMenu = ::GetMenu(m_hWnd);
	SetMenuInfo(hMenu, &mi);

}
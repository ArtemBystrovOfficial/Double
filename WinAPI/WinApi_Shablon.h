#pragma once

//INTERFACE
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//WINSOCK
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

// STL
#include <Windows.h>
#include <string>
#include <algorithm>
#include <exception>
#include <bitset>
#include <limits>
#include <deque>
#include <thread>
#include <fstream>
#include "Bridge_Interface_to_Server.h"

typedef const int& CINTR;

// CLASS OF INTERFACE ELEMENT
struct Object
{
	explicit Object(
	int x=0, int y=0,int width=60,int height=20,
	const std::wstring & o_class = L"NONE", const std::wstring& o_name = L"NONE"
	) : x(x),y(y),width(width),height(height),o_class(o_class),o_name(o_name) {};

	operator HWND() const
	{
		return object;
	}

	HWND object{};
	int x, y,height,width;
	std::wstring o_class, o_name;
};

// PARSER FOR JSON OR TXT SETTING
class Parser
{

public:

	Parser(const std::string PATH) : PATH(PATH) {}

	void Start();

	std::string IP;
	int PORT;
	std::wstring KEY;
	std::wstring NICKNAME;
	bool HOST = false;

private:

	std::string PATH;

};

// MESSAGE_LINE
struct Message_Line
{
	Message_Line(const wchar_t* w_str, const short& x, const short& y, const short& length);
	
	Message_Line(const Message_Line& ms);

	Message_Line& operator= (const Message_Line& ms);

	void Set_Name(const wchar_t * );


	
	wchar_t * message;
	short x, y, len;
	bool with_name = false;
	bool is_me = false;
	std::wstring name;


	~Message_Line();

};

// CHECK_CONNECT
struct Check_Signals
{
	static void set_marker(HDC& hdc)
	{
		hdc = hdc;
	}
	static void status()
	{
		HBRUSH hBrush;
		if (!Check_Signals::is_conected_with_server)
		{

			hBrush = CreateSolidBrush(RGB(200, 0, 0));
			SelectObject(hdc, hBrush);

		}
		else
		{

			hBrush = CreateSolidBrush(RGB(255, 255, 51));
			SelectObject(hdc, hBrush);

		}

		Rectangle(hdc, 50, 420, 35, 405);


		if (!Check_Signals::is_conected_with_client)
		{
			hBrush = CreateSolidBrush(RGB(200, 0, 0));
			SelectObject(hdc, hBrush);

		}
		else
		{
			hBrush = CreateSolidBrush(RGB(255, 255, 51));
			SelectObject(hdc, hBrush);

		}

		Rectangle(hdc, 80, 420, 65, 405);
	}
	static bool is_conected_with_server;
	static bool is_conected_with_client;
	static HDC hdc;
};

// CHAT_BUFFER
class Chat_Buffer
{
private:
	std::deque <Message_Line> Message_box;
	HWND Window_chat;
	HFONT	hFont;
	HDC hdc;

	int current_count_msg_buff = 0;
	int last_vertical_draw = 230;
	int Max_Count_Message = 10;
	int Max_Count_Letters = 60;
	int Move_Reg_Step = 20;
	int x = 100;
	std::wstring last_nick;
	bool diff_nicks;


	int Current_Move_Reg_Msg = 0;

	void Plus(Message_Line&);
	void Sliser_Message(const wchar_t*);

public:
	void add (const wchar_t* ,const wchar_t*);
	void print(const wchar_t*,const wchar_t*,bool = false);
	HDC & get_hdc()
	{
		return hdc;
	}

	explicit Chat_Buffer() = delete;
	Chat_Buffer(HWND& hWnd) : Window_chat{ hWnd } {}
	void Set_Window(HWND& hWnd) { Window_chat = hWnd; hdc = GetDC(hWnd); Check_Signals::hdc = hdc; }
	Chat_Buffer(CINTR x, int y, CINTR  width, CINTR  height, CINTR letter_space,HFONT & font) :
														x{ x },
														last_vertical_draw{ y }, 
														Max_Count_Message{ height / 40 },
														Max_Count_Letters{ width / 6 }, 
														Move_Reg_Step{ letter_space },
														hFont(font){
										while (Message_box.size() <= Max_Count_Message) 
												Message_box.push_back(Message_Line(L" ", x, last_vertical_draw, 1 ));
	}

};

// MAIN APP
class CApp
{
private:
	enum class CTL_ID {
		Send_Msg,
		Edit_Text
	};
public:
	explicit CApp();

	~CApp(){};

	int Run();


private:

	// System Functions
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	void init_native_window_obj();
	static LRESULT CALLBACK application_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void create_native_controls();

	// Ports Server
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	void Input_Port();
	
protected:

	// Main Functions
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	void Send_Msg(HWND&);



private: 


	// Flags
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	bool is_TextArea_Selected = false;

	// Meta Data
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
public:

	std::wstring nickname=L"GUEST";

	// Consts
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

private:

	const short Max_Message_Size = 512;

	// Elements_Visual_Settings;
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	const std::wstring	   m_szAppName			 { L"Double" },   
						   m_szClassName		 { L"Double_window_class_nostyles" };
	const int		       m_nAppWidth			 { 700 }, 
						   m_nAppHeight			 { 500 };
	Object			       m_hWnd				 { 0, 0, m_nAppWidth, m_nAppHeight },
						   m_hWndButton_Send_Msg { 600, 350, 50, 50,L"Button",L"\n<->"},
						   m_hWndTextArea_Msg	 { 100, 350, 500, 70, L"Edit", L"Try Connection"};
	HFONT				   hFont         =		 CreateFont(15, 0, 0, 0, 450, 0, 0, 0, DEFAULT_CHARSET,
																OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
																	CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Roboto");
	Chat_Buffer			   Chat_Msg				 { 100, 250, 400, 470, 20, hFont };
};


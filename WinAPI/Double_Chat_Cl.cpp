#include "WinApi_Shablon.h"


Message_Line::Message_Line(const wchar_t* w_str, const short& x, const short& y, const short& length) : x(x), y(y)
	{
		this->len = length;
		int w_size = wcslen(w_str);
		wchar_t* new_t = new wchar_t[size_t(w_size+int(1))];
		
		int p = w_size;
		for (int i=0;p--;i++)
		{
			*(new_t+i) = *(w_str+i);
		}
		new_t[w_size] = '\0';
			
		message = new_t;
	}
Message_Line::Message_Line(const Message_Line & ms) : x(ms.x), y(ms.y), len(ms.len)
	{

		int w_size = wcslen(ms.message);
		wchar_t* new_t = new wchar_t[size_t(w_size + int(1))];

		int p = w_size;
		for (int i = 0; p--; i++)
		{
			*(new_t + i) = *(ms.message + i);
		}
		new_t[w_size] = '\0';

		message = new_t;
	}
Message_Line & Message_Line::operator= (const Message_Line& ms)
	{
		len = ms.len;
		int w_size = wcslen(ms.message);
		wchar_t* new_t = new wchar_t[size_t(w_size + int(1))];

		int p = w_size;
		for (int i = 0; p--; i++)
		{
			*(new_t + i) = *(ms.message + i);
		}
		new_t[w_size] = '\0';

		message = new_t;
		return *this;
	}
Message_Line::~Message_Line()
	{
		delete message;
		message = nullptr;
	}

void Message_Line::Set_Name(const wchar_t * nickname)
{
	name = nickname;
	with_name = true;
}

bool next_is_me = false;

void Chat_Buffer::print(const wchar_t* nick_name ,const wchar_t* wstr_msg, bool is_me)
{
	if(current_count_msg_buff<Max_Count_Message)
								current_count_msg_buff++;
	next_is_me = is_me;

	HBRUSH hBrush;
	add(nick_name , wstr_msg);

	SelectObject(hdc, hFont);

	InvalidateRect(Window_chat, 0, TRUE);
	UpdateWindow(Window_chat);

	if (!Check_Signals::is_conected_with_server)
	{

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
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

	int kof_visibly = 240;

	Rectangle(hdc, 80, 420, 65, 405);

	SetBkMode(hdc, TRANSPARENT);
	

	for (int i = 0; i < Message_box.size(); i++)
	{

		if (Message_box[i].is_me)
		{
			SetTextAlign(hdc, TA_RIGHT);
			Message_box[i].x += 450;
		}

		SetTextColor(hdc, RGB(kof_visibly, kof_visibly, kof_visibly));
		if (Message_box[i].with_name && !Message_box[i].is_me)
		{
			TextOut(hdc, Message_box[i].x-80, Message_box[i].y, Message_box[i].name.c_str(), Message_box[i].name.size());
		}

		TextOut(hdc, Message_box[i].x, Message_box[i].y, Message_box[i].message, Message_box[i].len);
		kof_visibly -= 12;

		if (Message_box[i].is_me)
		{
			SetTextAlign(hdc, TA_LEFT);
			Message_box[i].x -= 450;
		}

	}
}

void Chat_Buffer::Plus(Message_Line & ml)
{
	ml.y = last_vertical_draw + Current_Move_Reg_Msg;
}

void Chat_Buffer::Sliser_Message(const wchar_t* wstr)
{
	std::wstring ws_str(wstr);
	std::wstring case_str;
	short length = ws_str.size();
	short count_let = 0;
	short last_begin = 0;
	short Last_Space_Id = 0;
	bool  is_Space_in_Line = false;
	bool first_box = true;

	// Каретка чата
	for (short i = 0; i < length; ++i)
	{	
		if (ws_str[i] == ' ')
		{
			is_Space_in_Line = true;
			Last_Space_Id = i;
		}
		if (ws_str[i] == '\n' || count_let >= Max_Count_Letters )
		{

			if (count_let >= Max_Count_Letters && is_Space_in_Line)
			{
					int retrn = Max_Count_Letters - Last_Space_Id;
					count_let -= retrn - 1; i -= retrn - 1;
			}
			case_str = ws_str.substr(last_begin, count_let);

			
			Message_Line New_Line(case_str.c_str(), x, last_vertical_draw, case_str.size());
			Message_box.push_front(New_Line);
			if (next_is_me) Message_box[0].is_me = true;
			if (first_box && diff_nicks) 
			{
				Message_box[0].Set_Name(last_nick.c_str());
			}

			count_let = 0;
			last_begin = i;
			is_Space_in_Line = false;
			first_box = false;
		}
		count_let++;
	}
	if (count_let)
	{
		case_str = ws_str.substr(last_begin, count_let);
		Message_Line New_Line(case_str.c_str(), x, last_vertical_draw, case_str.size());
		Message_box.push_front(New_Line);
		if (next_is_me) Message_box[0].is_me = true;
		if (first_box && diff_nicks)
		{
			Message_box[0].Set_Name(last_nick.c_str());
		}
	}
}

void Chat_Buffer::add(const wchar_t* nick_name , const wchar_t* w_str)
{
	
	diff_nicks = std::wstring(nick_name) != last_nick.c_str() ? true : false;
	last_nick = nick_name;

	Message_box.push_front(Message_Line(L"\0", x, last_vertical_draw, 0));

	Sliser_Message(w_str);

	if (Message_box.size() > Max_Count_Message)
		Message_box.pop_back();
	Current_Move_Reg_Msg = 0;
	for (int i = 0; i < Message_box.size(); i++)
	{
		
		Plus(Message_box[i]);
		Current_Move_Reg_Msg -= Move_Reg_Step;
	}
}
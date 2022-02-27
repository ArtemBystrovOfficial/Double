#pragma once

#include <string>
#include <deque>
#include <string>

struct ServerData
{
	wchar_t msg[512];
	wchar_t nickname[16];
};

class Bridge_IS
{
public:

	static void Script(wchar_t value[], const wchar_t * key)
	{
		int bar = 0;
		int length = wcslen(value);
		int key_length = wcslen(key);
		if (!length)return;
		for (int i = 0; i < length; i++)
		{
			if (value[i] != '\0') {
				value[i] = value[i] ^ key[bar];
				if (++bar >= key_length)bar = 0;
			}
		}
	}


	static void conv_msg_to_ServData(wchar_t wstr_to[], const wchar_t* wstr_from)
	{
		
		int length = wcsnlen(wstr_from,512);
		for (int i = 0; i < length; i++)
		{
			wstr_to[i] = wstr_from[i];
		}
		wstr_to[length] = '\0';
	}

	static std::wstring conv_ServData_to_msg(const wchar_t wstr_from[])
	{
		std::wstring data;
		for (int i = 0; wstr_from[i] != '\0'; i++)
		{
			data.push_back(wstr_from[i]);
		}
		data.push_back('\0');
		return data;
	}

	static void Bridge_ItS(ServerData & packet)
	{
		Script(packet.msg, key.c_str());
		Script(packet.nickname, key.c_str());
		S_OUT.push_back(packet);
	}
	static void Bridge_StI(ServerData& packet)
	{
		Script(packet.msg, key.c_str());
		Script(packet.nickname, key.c_str());
		S_IN.push_back(packet);
	}
	static std::wstring key;
	static std::deque < ServerData > S_IN;
	static std::deque < ServerData > S_OUT;
};


#include "WinApi_Shablon.h"

void Parser::Start()
{
	std::ifstream in(PATH);

	std::string s;
	int n;

	in >> s;

	if (!(s == "MODE")) throw std::exception("Uncorrect MODE Syntax");

	in >> s;

	if (!(s == ":")) throw std::exception("Uncorrect MODE, don't find -> :");

	in >> s;

	if (s == "HOST")
	{
		HOST = true;
	} else if (s == "CLIENT") HOST = false;
		else throw std::exception("Uncorrect MODE -> CLIENT | HOST");


	in >> s;

	if (!(s == "IP")) throw std::exception("Uncorrect IP Syntax");

	in >> s;

	if (!(s == ":")) throw std::exception("Uncorrect IP, don't find -> :");

	in >> s;

	if (s.size() < 21 && s.size() > 5 )
	{
		int k = 0;
		bool is_normal=false;
		for (auto i : s)
		{
			if (i == '.') {
				k++;  is_normal = true;
			}
			if (i>=48 || i<=59)is_normal = true;
		}
		if (k != 3 || !is_normal) throw std::exception("Uncorrect IP");
	}
	else
	{
		throw std::exception("Uncorrect IP length");
	}

	IP = s;

	in >> s;

	if (!(s == "PORT")) throw std::exception("Uncorrect PORT Syntax");

	in >> s;

	if (!(s == ":")) throw std::exception("Uncorrect PORT, don't find -> :");

	in >> n;

	if (!(n < 1000000 && n > 100)) throw std::exception("Uncorrect PORT length");

	PORT = n;

	in >> s;

	if (!(s == "KEY")) throw std::exception("Uncorrect KEY Syntax");

	in >> s;

	if (!(s == ":")) throw std::exception("Uncorrect KEY, don't find -> :");

	in >> s;

	if (!(s.size() < 50 && s.size() > 2)) throw std::exception("Uncorrect KEY >50 or <2 length");

	std::wstring wstr(s.begin(), s.end());

	KEY = wstr;

	in >> s;

	if (!(s == "NICKNAME")) throw std::exception("Uncorrect NICKNAME Syntax");

	in >> s;

	if (!(s == ":")) throw std::exception("Uncorrect NICKNAME, don't find -> :");

	in >> s;

	wstr = std::wstring(s.begin(), s.end());

	if (!(wstr.size() < 15 && wstr.size() >= 4)) throw std::exception("Uncorrect NICKNAME length");
	
	NICKNAME = wstr;

}
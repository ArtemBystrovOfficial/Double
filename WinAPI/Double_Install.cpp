#include "WinApi_Shablon.h"

bool Check_Signals::is_conected_with_server = false;
bool Check_Signals::is_conected_with_client = false;
HDC Check_Signals::hdc{};

CApp::CApp()
{
	using std::wstring;
	using std::string;
	try
	{
		this->init_native_window_obj();
		this->create_native_controls();

		std::thread in_thread(&CApp::Input_Port, this);
		in_thread.detach();

	}
	catch (const std::exception& ex)
	{
		string expt_data = ex.what();

		MessageBox(nullptr, wstring(begin(expt_data), end(expt_data)).c_str() , L"Error", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
		exit(0);
	}
}

void CApp::init_native_window_obj()
{

	using namespace std::string_literals;

	WNDCLASSEX _wc{ sizeof(WNDCLASSEX) };
	_wc.cbClsExtra = 0;
	_wc.cbWndExtra = 0;
	_wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(40, 40, 40)));
	_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	_wc.hIconSm = LoadIcon(nullptr, IDI_SHIELD);
	_wc.hInstance = GetModuleHandle(nullptr);
	_wc.lpfnWndProc = CApp::application_proc;
	_wc.lpszClassName = this->m_szClassName.c_str();
	_wc.lpszMenuName = nullptr;
	_wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&_wc))
		throw std::exception("Register Window Error");

	RECT _windowRC{ 0,0,this->m_nAppWidth,this->m_nAppHeight};
	AdjustWindowRect(&_windowRC, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE, false);
	

		this->m_hWnd.object = CreateWindowEx(
			0, this->m_szClassName.c_str(),
			this->m_szAppName.c_str(),
			WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
			(GetSystemMetrics(SM_CXSCREEN) - _windowRC.right) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - _windowRC.bottom) / 2,
			_windowRC.right, _windowRC.bottom, nullptr, nullptr, nullptr, this
		);
	
		Chat_Msg.Set_Window(m_hWnd.object);

	if (!this->m_hWnd.object) throw std::exception("Error,can't creat main window");
}

int CApp::Run()
{
	MSG _msg{};

	ShowWindow(this->m_hWnd.object, SW_SHOWDEFAULT);

	UpdateWindow(this->m_hWnd.object);

	Check_Signals::status();

	while (GetMessage(&_msg, nullptr, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}

	return static_cast<int>(_msg.wParam);
}



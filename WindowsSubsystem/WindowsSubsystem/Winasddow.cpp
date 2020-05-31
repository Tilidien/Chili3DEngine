#include <Window.h>

Window::WindowClass Window::WindowClass::wndClass; //This looks dumb, cant i change that to Window::WindowClass::wndClass; ???
Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);				//Size of this structure in bytes
	wc.style = SC_OWNDC;				//ClassStyle. can have multiple class styles. see https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
	wc.lpfnWndProc = HandleMsgSetup;	//"Long Pointer to Funktion for Windows Procs" Pointer to member that handels os Events/Messages
	wc.cbClsExtra = 0;					//int Number of extra bytes to the class structure. Zero for now."wen dont need that"
	wc.cbWindExtra = 0;					//int NUmber of extra bytes to the instances of each window.Not needed
	wc.hInstance = GetInstance();		//hInstance we already got from os
	wc.hIcon = nullptr;					//custom class icon
	wc.hCurso = nullptr;				//Custom cursor
	wc.hbrBackground = nullptr;			//Brush to be used by os to draw WIndow Backround. set to null since we gona draw everything ourselfes
	wc.lpsMenuName = nullptr;			//Pointer to null terminated char string that specifies resource name of the class menu. Not needed, since we do all ourselfes
	wc.lpszClassName = GetName();		//Class name used to give to os when creating WIndows of this class type
		wc.hIconSM = nullptr;			//Icon for the Application
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance()); //###############Cant i use hInst instead of GetInstance?
}
const char* Window::WindowClass::GetName() noexcept
{
	return wndCLassName;
}
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* name = noexcept
{
	//clalculate actualy client region
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE);
	//Create WIndow & get hWnd
	hWind = CreateWindowEx(
	0,					//DWORD 	dwExStyle more styles. see https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
	pClassName,			//LPCSTR 	lpClassName classname that we already given to our registered window
	"And cute girls",	//LPCSTR	lpWindowName Name for the Window
	WS_CAPTION | 		//DWORD		dwStyle more styles. see https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
	WS_MINIMIZEBOX |
	WS_SYSMENU,
	CW_USEDEFAULT,					//int 		x 
	CW_USEDEFAULT,					//int 		y
	wr.right - wr.left,				//int		nWidth
	wr.bottom - wr.top,				//int		nHeight
	nullptr,						//HWND		hWndParent Handle to the Parent			
	nullptr,						//HMENU 	no menu here
	WindowClass::GetInstance(),		//HINSTANCE passed to us from os
	this);							//lpParam	parameter to config custom data that is passed to out window
	ShowWindow(hWind,SW_SHOWDEFAULT);
}

WINDOW::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		//extract pointer to Windows class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam)
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams); // why ist const called after type?
		//Set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_GWLP_USERDATA, reinterpret_cast<Long_PTR>(pWnd));
		//set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpete_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msgmwParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
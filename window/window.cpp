#include "window.h"

void BoolApp::PWindow::construction()
{
	hwnd = CreateWindowEx(
		WS_EX_TRANSPARENT,
		view->getSzWindowClass().c_str(),
		L"BoolApp",
		WS_VISIBLE | (WS_OVERLAPPED | /*WS_CAPTION | WS_SYSMENU |*/ WS_THICKFRAME/* | WS_MINIMIZEBOX*/ | WS_MAXIMIZEBOX) | WS_CLIPCHILDREN | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
		hwnd,
		0,
		instance,
		0
	);
	SetWindowLongPtr(hwnd, 0, (LONG_PTR) this);
};

LRESULT BoolApp::Window::wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam, ProcessView *ptr)
{
	PWindow* pWindow = (PWindow*)ptr;

	return DefWindowProc(ahwnd, message, wparam, lparam);
};

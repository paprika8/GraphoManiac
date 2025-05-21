#include "Window.h"
#include "positioner.h"

namespace Graphs
{

	HINSTANCE instance = 0;

	std::wstring Window::getSzWindowClass()
	{
		return L"Wind";
	}

	void Window::regist()
	{
		WNDCLASS wca = { 0 };
		// адрес ф-ции обработки сообщений
		wca.lpfnWndProc = (WNDPROC)wndProc;
		// стиль окна
		wca.style = CS_HREDRAW | CS_VREDRAW;
		// дискриптор экземпляра приложения
		wca.hInstance = instance;
		// название класса
		wca.lpszClassName = getSzWindowClass().c_str();
		// загрузка курсора
		wca.hCursor = LoadCursor(NULL, IDC_ARROW);
		// загрузка цвета окон
		wca.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
		// резерв места для указателя на View
		wca.cbWndExtra = sizeof(void*);
		// регистрация класса
		RegisterClass(&wca);
	}
	void Window::show(int type)
	{
		if(hwnd){
			ShowWindow(hwnd, type);
			return;
		}
		hwnd = CreateWindowEx(
			WS_EX_TRANSPARENT,
			getSzWindowClass().c_str(),
			L"Graphs",
			WS_VISIBLE | (WS_OVERLAPPED | /*WS_CAPTION | WS_SYSMENU |*/ WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX) | WS_CLIPCHILDREN | WS_BORDER,
			CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
			hwnd,
			0,
			instance,
			0
		);
		SetWindowLongPtr(hwnd, 0, (LONG_PTR) this);
		ShowWindow(hwnd, type);
		RECT rc;
		GetClientRect(hwnd, &rc);
		size = Size_(rc.right, rc.bottom);
		screen->resize(rc.right, rc.bottom);
		InvalidateRect(hwnd, 0, 0);
	}

	using enum mouse_buttons;
	using enum click_event;

	LRESULT Window::wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		Window *ptr = GetWindowLongPtr(ahwnd, 0);
		switch (message)
		{
		case WM_ERASEBKGND:
    		return 1;
		case WM_PAINT:
		{
			PAINTSTRUCT pStruct;
			HDC hdc = GetDC(ahwnd);
			BufferHDC buffer = BufferHDC(hdc, ptr->size);
			ptr->screen->paint(buffer);
			ValidateRect(ahwnd, 0);
			return 0;
		}
		case WM_NCCREATE:
		{
			return DefWindowProc(ahwnd, message, wparam, lparam);
		}
		case WM_NCDESTROY:
		{ // конец удаления
			HWND parentHWND = GetParent(ahwnd);
			if (parentHWND)
			{
				InvalidateRect(parentHWND, 0, 0);
			}
		}
		case WM_DESTROY:
		{ // начало удаления
			if (ptr)
			{
				SetWindowLongPtr(ahwnd, 0, 0);
				ptr->hwnd = 0;
				delete ptr;
			}
			return DefWindowProc(ahwnd, message, wparam, lparam);
		}
		case WM_SIZE:
		{
			if (!ptr)
			{
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			//if (GetWindowLong(ahwnd, GWL_STYLE) & WS_SIZEBOX)
			ptr->screen->resize(LOWORD(lparam), HIWORD(lparam));
			ptr->size = Size_(LOWORD(lparam), HIWORD(lparam));
			Positioning(ptr->screen);
			return DefWindowProc(ahwnd, message, wparam, lparam);
		}
		case WM_MOVE:
		{
			if (!ptr)
			{
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			//ptr->screen->move(LOWORD(lparam), HIWORD(lparam));
			return DefWindowProc(ahwnd, message, wparam, lparam);
		}
		case WM_LBUTTONDBLCLK:
		if (ptr) ptr->screen->mouse_event(L, dbl, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;
		case WM_LBUTTONDOWN:
		if (ptr) ptr->screen->mouse_event(L, down, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;

		case WM_LBUTTONUP:
		if (ptr) ptr->screen->mouse_event(L, up, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;

		case WM_RBUTTONDBLCLK:
		if (ptr) ptr->screen->mouse_event(R, dbl, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;
		case WM_RBUTTONDOWN:
		if (ptr) ptr->screen->mouse_event(R, down, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;

		case WM_RBUTTONUP:
		if (ptr) ptr->screen->mouse_event(R, up, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;

		case WM_MBUTTONDBLCLK:
		if (ptr) ptr->screen->mouse_event(M, dbl, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;
		case WM_MBUTTONDOWN:
		if (ptr) ptr->screen->mouse_event(M, down, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;

		case WM_MBUTTONUP:
		if (ptr) ptr->screen->mouse_event(M, up, LOWORD ( lparam ), HIWORD ( lparam ), wparam);
		break;

		case WM_XBUTTONDBLCLK:
		{
			mouse_buttons but = X1;
			if(HIWORD(wparam) - 1)
				but = X2;
			if (ptr) ptr->screen->mouse_event(but, dbl, LOWORD ( lparam ), HIWORD ( lparam ), LOWORD(wparam));
		}
		break;
		case WM_XBUTTONDOWN:
		{
			mouse_buttons but = X1;
			if(HIWORD(wparam) - 1)
				but = X2;
			if (ptr) ptr->screen->mouse_event(but, down, LOWORD ( lparam ), HIWORD ( lparam ), LOWORD(wparam));
		}
		break;

		case WM_XBUTTONUP:
		{
			mouse_buttons but = X1;
			if(HIWORD(wparam) - 1)
				but = X2;
			if (ptr) ptr->screen->mouse_event(but, up, LOWORD ( lparam ), HIWORD ( lparam ), LOWORD(wparam));
		}
		break;

		case WM_MOUSEMOVE:
		if (ptr) ptr->screen->mouse_move_event(LOWORD ( lparam ), HIWORD ( lparam ), wparam);

		case WM_MOUSELEAVE:
		if (ptr) ptr->screen->mouse_leave_event();

		case WM_MOUSEACTIVATE:

		case WM_MOUSEWHEEL:
		if (ptr) ptr->screen->mouse_wheel_event(LOWORD ( lparam ), HIWORD ( lparam ), HIWORD(wparam), LOWORD(wparam));

		case WM_CHAR:{
			if (ptr) ptr->screen->char_event(lparam, wparam);
			return 0;
		}

		case WM_KEYUP:{
			struct_key_event st = lparam;
			st.transition_state = 1;
			st.is_first_message = 0;
			if (ptr) ptr->screen->key_event(st, wparam);//VK_ codes
            return 0;
			break;
		}
		case WM_KEYDOWN: {
			struct_key_event st = lparam;
			st.transition_state = 0;
			st.is_first_message = 1;
			if (ptr) ptr->screen->key_event(st, wparam);//VK_ codes
            return 0;
			break;
    }

		case WM_KILLFOCUS:
		if (ptr) ptr->screen->kill_focus_event();
		return 0;
		case WM_SETFOCUS:
		if (ptr) ptr->screen->set_focus_event();
		return 0;
		
		default:
		{
			if (!ptr)
			{
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			//return ptr->view->wndProc(ahwnd, message, wparam, lparam, ptr);
			return DefWindowProc(ahwnd, message, wparam, lparam);
		}
		}
		return DefWindowProc(ahwnd, message, wparam, lparam);
	}
}
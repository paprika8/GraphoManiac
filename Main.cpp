#include "Main.h"
#include <winuser.h>

#include "Button.h"
#include "graphs/draw_graph.h"
#include "Text.h"

UINT GetSystemDpi() {
	HDC hdc = GetDC(NULL);
	UINT dpi = GetDeviceCaps(hdc, LOGPIXELSX);
	ReleaseDC(NULL, hdc);
	return dpi;
}

namespace Graphs
{
	Window* win = 0;
	int DPI = 0;
}


ULONG_PTR gdiplusToken = 0;

int run() {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR lpCmdLine, int nshow) {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

	::instance = instance;
	Gdiplus::Status st = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	if (st != Gdiplus::Ok) {
		MessageBox(NULL,
				   L"Call to GdiplusStartup failed!",
				   L"GdiplusStartup ist kaputt",
				   NULL);
		return -1;
	}

	//создание окна
	win = new Window();

	//...
	DPI = GetSystemDpi();
	//регистрация окна
	win->regist();
	//создание и отображение окна
	win->show(SW_NORMAL);

	//получаем ссылку на screen из win
	Composite* screen = dynamic_cast<Composite*>(win->screen);
	//задаём выравнивание дочерних элементов по центру
	screen->margin.type |= MarginType::HCENTER | MarginType::VCENTER;

	//создаём кнопку
	Button* bt1 = new Button(screen);
	bt1->size = Size_(100, 100);
	bt1->margin = Margin(10, 10, 10, 10);
	screen->add(bt1);

	//создаём кнопку
	Button* bt2 = new Button(screen);
	bt2->size = Size_(100, 100);
	bt2->margin = Margin(10, 10, 10, 10);
	screen->add(bt2);

	//создаём текстовый блок
	GraphView* tx = new GraphView(screen);
	tx->size = Size_(200, 200);
	tx->margin = Margin(10, 10, 10, 10);
	screen->add(tx);

	//отображаем окно снова... так работает лучше наверное...
	win->show(SW_NORMAL);

	int res = run();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return res;
}
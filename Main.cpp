#include "Main.h"
#include <winuser.h>

#include "Button.h"
#include "graphs/draw_graph.h"
#include "Text.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"
#include "page4.h"
#include "page5.h"
#include "page6.h"
#include "page7.h"
#include "page8.h"
#include "page9.h"
#include "page10_11.h"
#include "page12.h"

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
	bt1->size = Size_(pointUI(90, percent), pointUI(1000, fr));
	bt1->margin = Margin(pointUI(5, percent), 10, pointUI(5, percent), 10);
	screen->add(bt1);

	//создаём кнопку
	Button* bt2 = new Button(screen);
	bt2->size = Size_(pointUI(90, percent), pointUI(1000, fr));
	bt2->margin = Margin(pointUI(5, percent), 10, pointUI(5, percent), 10);
	screen->add(bt2);

	//создаём текстовый блок
	/*MassGraphView* tx = new MassGraphView(screen);
	tx->size = Size_(pointUI(800, percent), pointUI(1000, percent));
	//tx->margin = Margin(10,10,10,10);
	screen->add(tx);*/

	create_p1();

	//отображаем окно снова... так работает лучше, наверное...
	win->show(SW_NORMAL);

	int res = run();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return res;
}
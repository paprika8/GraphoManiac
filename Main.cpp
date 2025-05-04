#include "Main.h"
#include <winuser.h>
UINT GetSystemDpi(){
	HDC hdc = GetDC(NULL);
	UINT dpi = GetDeviceCaps(hdc, LOGPIXELSX);
	ReleaseDC(NULL, hdc);
	return dpi;
}
Window* win = 0;

ULONG_PTR gdiplusToken = 0;

int DPI = 0;

LinearContainer* create_page_main(){
	LinearContainer* lc = new LinearContainer(new SizeBuilder(Size(pointUI(1000, percent), pointUI(1000, percent)), Margin(0), Padding(0)));
	lc->is_vert_orientation = 1;
	lc->background = bg;
	

	ScrollText* app_name = new ScrollText(new SizeBuilder(Size(pointUI(400, percent), pointUI(350, percent)), Margin(0), Padding(pointUI(150, percent), pointUI(100, percent), 0, 0)));
	//SetWindowLongPtr(app_name->PV->hwnd, GWL_STYLE, WS_VISIBLE + WS_CHILD + BS_OWNERDRAW);
	app_name->SetText(L"B001App");
	app_name->font = get_font(100);
	app_name->background = trans;
	app_name->text_color = dark_t;
	lc->add(app_name);

	Button* exer_bt = new Button(new SizeBuilder(Size(pointUI(250), pointUI(80)), Margin(5, 5, 5, 5, MarginType::LEFT | MarginType::TOP | MarginType::CONTENT), Padding(pointUI(10, percent), 0, 0, 0)));
	exer_bt->click = [&](Button*)->void{};
	exer_bt->text = L"ЗАДАЧИ";
	exer_bt->set_font_size(35);
	exer_bt->background = button;
	exer_bt->text_color = light_t;
	lc->add(exer_bt);

	Button* games_bt = new Button(new SizeBuilder(Size(pointUI(250), pointUI(80)), Margin(5, 5, 5, 5), Padding(pointUI(10, percent), 0, 0, 0)));
	games_bt->click = [&](Button*)->void{};
	games_bt->text = L"ИГРЫ";
	games_bt->set_font_size(35);
	games_bt->background = button;
	games_bt->text_color = light_t;
	lc->add(games_bt);

	Button* close_bt = new Button(new SizeBuilder(Size(pointUI(100, percent), pointUI(80, percent)), Margin(5, 5, 5, 5), Padding(pointUI(10, percent), 0, 0, 0)));
	close_bt->click = [](Button*)->void{PostQuitMessage ( 0 );};
	close_bt->text = L"ВЫХОД";
	close_bt->set_font_size(35);
	close_bt->background = button;
	close_bt->text_color = light_t;

	close_bt->text_resizer = 1;
	close_bt->stringFormat->SetFormatFlags(Gdiplus::StringFormatFlags::StringFormatFlagsNoWrap | close_bt->stringFormat->GetFormatFlags());
	lc->add(close_bt);

	/*
	Edit* text = new Edit(new SizeBuilder(Size(pointUI(100), pointUI(60)), Margin(5, 5, 5, 5), Padding(0,0,20,0)));
	text->SetText(L"TEXT УБОГИЙ");
	lc->add(text);
	*/


	return lc;
}

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR lpCmdLine, int nshow) {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

	Gdiplus::Status st = Gdiplus::GdiplusStartup ( &gdiplusToken , &gdiplusStartupInput , NULL );
	if ( st != Gdiplus::Ok )
	{
		MessageBox ( NULL ,
			L"Call to GdiplusStartup failed!" ,
			L"GdiplusStartup ist kaputt" ,
			NULL );
		return -1;
	}


	win = new Window (new SizeBuilder(Size(500, 500), Margin(0,0,0,0, MarginType::TOP | MarginType::LEFT), Padding(0)));
	win->Show(ShowType::MAXIMIZE);
	DPI = GetSystemDpi();

	win->add(create_page_main());

	int res = run();
	Gdiplus::GdiplusShutdown ( gdiplusToken );
	return res;
}
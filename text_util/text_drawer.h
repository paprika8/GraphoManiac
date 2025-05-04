#pragma once
#include "../oopWin.h"
#include "../custom_wstr.h"
#include "../custom_vector.h"
#include <map>

namespace Graphs
{

	struct text_model{
		short text_height = 0;
		custom_vector<custom_vector<short>> metriks;
	};

	extern const wchar_t* fonts_name[];

	extern std::map<std::pair<int, int>, Gdiplus::Font*> fonts;

	Gdiplus::Font* create_font(int size, int id);
	Gdiplus::Font* get_font(int size, int id = 0);

	void rect_text(custom_wstr text, HDC hdc, HFONT font, Gdiplus::Rect rect, Gdiplus::Rect& res);
	Size get_size(custom_wstr text, Gdiplus::StringFormat* format, int width, int size);
	Size get_size(Gdiplus::Graphics* g, custom_wstr text, Gdiplus::StringFormat* format, int width, int size);

	int generate_text_size(HWND hwnd, custom_wstr text, Gdiplus::StringFormat* format, int size, Size fill_size);

	text_model generate_model(HDC hdc, custom_wstr text);

	void insert_to_model(HDC hdc, text_model &model, wchar_t a, int corsorX, int corsorY);
	void erase_to_model(HDC hdc, RECT rect, text_model &model, int corsorX, int corsorY);
	int cursor_from_model(text_model &model, int X, int Y);
}
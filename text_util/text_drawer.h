#pragma once

#include "../Views.h"

#include <map>
#include <vector>
#include <string>


namespace Graphs
{

	extern const wchar_t* fonts_name[];

	extern std::map<std::pair<int, int>, Gdiplus::Font*> fonts;

	Font* create_font(int size, int id);
	Font* get_font(int size, int id = 0);

	void rect_text(std::wstring text, HDC hdc, HFONT font, Gdiplus::Rect rect, Gdiplus::Rect& res);
	Size get_size(std::wstring text, Gdiplus::StringFormat* format, int width, int size);
	Size get_size(Gdiplus::Graphics* g, std::wstring text, Gdiplus::StringFormat* format, int width, int size);

	int generate_text_size_no_wrap(Gdiplus::Graphics *g, std::wstring text, Gdiplus::StringFormat* format, int size, Size_ fill_size);

	//сомнительная штука, нерекомендуется для использования
	int generate_text_size(HWND hwnd, std::wstring text, Gdiplus::StringFormat* format, int size, Size_ fill_size);
}
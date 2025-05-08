#include "text_drawer.h"
#include "../Main.h"

namespace Graphs{
	const wchar_t* fonts_name[] = { L"Times New Roman", L"Consolas" };
	std::map<std::pair<int, int>, Gdiplus::Font*> fonts;
	Gdiplus::Font* create_font(int size, int id)  {
		Gdiplus::FontFamily family(fonts_name[id]);
		return new Gdiplus::Font(&family, size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	}
	Gdiplus::Font* get_font(int size, int id) {
		if(!fonts[{size, id}])
			fonts[{size, id}] = create_font(size, id);
		return fonts[{size, id}];
	}
	void rect_text(std::wstring text, HDC hdc, HFONT font, Gdiplus::Rect rect, Gdiplus::Rect& res) {
		const wchar_t* it = text.c_str();
		int row = rect.Y, cursor = rect.X;
		SelectObject(hdc, font);
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		int stepY = tm.tmHeight;
		while (*it) {
			ABC abc;
			GetCharABCWidthsW(hdc, *it, *it, &abc);
			cursor += abc.abcA;
			if (cursor + abc.abcB > rect.X + rect.Width) {
				cursor = rect.X + abc.abcA;
				row += stepY + 10;
			}
			cursor += abc.abcB;
			cursor += abc.abcC;
			it++;
		}
		row += stepY + 10;
		res.Height = row - rect.Y;
		res.Width = rect.Width;
		res.Y = rect.Y;
		res.X = rect.X;
	}
	Size get_size(std::wstring text, Gdiplus::StringFormat* format, int width, int size) {
		if(width == -1)
			width = 100000;
		Gdiplus::RectF r_in(0., 0., width, 100000.);
		Gdiplus::RectF r_out;
		Gdiplus::Graphics g(GetDC(0));
		g.MeasureString(text.c_str(), -1, get_font(size), r_in, format, &r_out);
		return Size(r_out.Width, r_out.Height);
	}
	Size get_size(Gdiplus::Graphics* g, std::wstring text, Gdiplus::StringFormat* format, int width, int size) {
		Gdiplus::RectF r_in(0., 0., width, 100000.);
		Gdiplus::RectF r_out;
		g->MeasureString(text.c_str(), -1, get_font(size), r_in, format, &r_out);
		return Size(r_out.Width, r_out.Height);
	}
	int generate_text_size_no_wrap(Gdiplus::Graphics *g, std::wstring text, Gdiplus::StringFormat* format, int size, Size_ fill_size){
		Gdiplus::RectF r_in(0., 0., 100000., 100000.);
		Gdiplus::RectF r_out;
		int h_size = fill_size.height * 72 / DPI;
		g->MeasureString(text.c_str(), -1, get_font(size), r_in, format, &r_out);
		size = size * r_out.Width / fill_size.width;
		return min(size, h_size);
	}
	int generate_text_size(HWND hwnd, std::wstring text, Gdiplus::StringFormat* format, int size, Size_ fill_size) {
		Gdiplus::Graphics g(GetDC(hwnd));
		if(format->GetFormatFlags() | Gdiplus::StringFormatFlags::StringFormatFlagsNoWrap)
			return generate_text_size_no_wrap(&g, text, format, size, fill_size);
		Gdiplus::RectF r_in(0., 0., fill_size.width, 100000.);
		Gdiplus::RectF r_out;
		g.MeasureString(text.c_str(), -1, get_font(size), r_in, format, &r_out);
		int last_size = size;
		int next_size;
		if(r_out.Height > fill_size.height){
			last_size = size / 2;
			next_size = size;
			g.MeasureString(text.c_str(), -1, get_font(last_size), r_in, format, &r_out);
			while(r_out.Height > fill_size.height){
				next_size = last_size;
				last_size = last_size / 2;
				if(last_size <= 1)
					return 1;
				g.MeasureString(text.c_str(), -1, get_font(last_size), r_in, format, &r_out);
			}
		}
		else{
			next_size = size * 2;
			g.MeasureString(text.c_str(), -1, get_font(next_size), r_in, format, &r_out);
			while(r_out.Height < fill_size.height){
				last_size = next_size;
				next_size = next_size * 2;
				if(next_size == 0)
					return 1;
				g.MeasureString(text.c_str(), -1, get_font(next_size), r_in, format, &r_out);
			}
		}
		int mid_s;
		do{
			mid_s = (last_size + next_size) / 2;
			g.MeasureString(text.c_str(), -1, get_font(mid_s), r_in, format, &r_out);
			if(r_out.Height > fill_size.height)
				next_size = mid_s;
			else
				last_size = mid_s;
		} while(next_size > last_size + 1);
		while(1){
			g.MeasureString(text.c_str(), -1, get_font(last_size), r_in, format, &r_out);
			if(r_out.Height > fill_size.height)
				last_size--;
			else break;
		}
		return last_size;
	}
}
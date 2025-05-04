#include "text_drawer.h"

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
	void rect_text(custom_wstr text, HDC hdc, HFONT font, Gdiplus::Rect rect, Gdiplus::Rect& res) {
		wchar_t* it = text.data;
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
	Size get_size(custom_wstr text, Gdiplus::StringFormat* format, int width, int size) {
		if(width == -1)
			width = 100000;
		Gdiplus::RectF r_in(0., 0., width, 100000.);
		Gdiplus::RectF r_out;
		Gdiplus::Graphics g(GetDC(0));
		g.MeasureString(text.data, -1, get_font(size), r_in, format, &r_out);
		return Size(r_out.Width, r_out.Height);
	}
	Size get_size(Gdiplus::Graphics* g, custom_wstr text, Gdiplus::StringFormat* format, int width, int size) {
		Gdiplus::RectF r_in(0., 0., width, 100000.);
		Gdiplus::RectF r_out;
		g->MeasureString(text.data, -1, get_font(size), r_in, format, &r_out);
		return Size(r_out.Width, r_out.Height);
	}
	int generate_text_size_no_wrap(Gdiplus::Graphics *g, custom_wstr text, Gdiplus::StringFormat* format, int size, Size fill_size){
		Gdiplus::PointF p(0., 0.);
		Gdiplus::RectF r_out;
		g->MeasureString(text.data, -1, get_font(size), p, format, &r_out);
		int last_size = size;
		int next_size;
		if(r_out.Height > fill_size.height || r_out.Width > fill_size.width){
			last_size = size / 2;
			next_size = size;
			g->MeasureString(text.data, -1, get_font(last_size), p, format, &r_out);
			while(r_out.Height > fill_size.height || r_out.Width > fill_size.width){
				next_size = last_size;
				last_size = last_size / 2;
				if(last_size <= 1)
					return 1;
				g->MeasureString(text.data, -1, get_font(last_size), p, format, &r_out);
			}
		}
		else{
			next_size = size * 2;
			g->MeasureString(text.data, -1, get_font(next_size), p, format, &r_out);
			while(r_out.Height < fill_size.height && r_out.Width < fill_size.width){
				last_size = next_size;
				next_size = next_size * 2;
				if(next_size == 0)
					return 1;
				g->MeasureString(text.data, -1, get_font(next_size), p, format, &r_out);
			}
		}
		int mid_s;
		do{
			mid_s = (last_size + next_size) / 2;
			g->MeasureString(text.data, -1, get_font(mid_s), p, format, &r_out);
			if(r_out.Height > fill_size.height || r_out.Width > fill_size.width)
				next_size = mid_s;
			else
				last_size = mid_s;
		} while(next_size > last_size + 1);
		while(1){
			g->MeasureString(text.data, -1, get_font(last_size), p, format, &r_out);
			if(r_out.Height > fill_size.height || r_out.Width > fill_size.width)
				last_size--;
			else break;
		}
		return last_size;
	}
	int generate_text_size(HWND hwnd, custom_wstr text, Gdiplus::StringFormat* format, int size, Size fill_size) {
		Gdiplus::Graphics g(GetDC(hwnd));
		if(format->GetFormatFlags() | Gdiplus::StringFormatFlags::StringFormatFlagsNoWrap)
			return generate_text_size_no_wrap(&g, text, format, size, fill_size);
		Gdiplus::RectF r_in(0., 0., fill_size.width, 100000.);
		Gdiplus::RectF r_out;
		g.MeasureString(text.data, -1, get_font(size), r_in, format, &r_out);
		int last_size = size;
		int next_size;
		if(r_out.Height > fill_size.height){
			last_size = size / 2;
			next_size = size;
			g.MeasureString(text.data, -1, get_font(last_size), r_in, format, &r_out);
			while(r_out.Height > fill_size.height){
				next_size = last_size;
				last_size = last_size / 2;
				if(last_size <= 1)
					return 1;
				g.MeasureString(text.data, -1, get_font(last_size), r_in, format, &r_out);
			}
		}
		else{
			next_size = size * 2;
			g.MeasureString(text.data, -1, get_font(next_size), r_in, format, &r_out);
			while(r_out.Height < fill_size.height){
				last_size = next_size;
				next_size = next_size * 2;
				if(next_size == 0)
					return 1;
				g.MeasureString(text.data, -1, get_font(next_size), r_in, format, &r_out);
			}
		}
		int mid_s;
		do{
			mid_s = (last_size + next_size) / 2;
			g.MeasureString(text.data, -1, get_font(mid_s), r_in, format, &r_out);
			if(r_out.Height > fill_size.height)
				next_size = mid_s;
			else
				last_size = mid_s;
		} while(next_size > last_size + 1);
		while(1){
			g.MeasureString(text.data, -1, get_font(last_size), r_in, format, &r_out);
			if(r_out.Height > fill_size.height)
				last_size--;
			else break;
		}
		return last_size;
	}
	text_model generate_model(HDC hdc, custom_wstr text) {
		text_model model;
		{
			custom_vector<short> line;
			line.push(0);
			model.metriks.push(line);
		}
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		model.text_height = tm.tmHeight;
		int stepY = tm.tmHeight;
		int itY = 0, itX = 0;
		int cursorX = 0;
		wchar_t* it = text.data;
		while(*it){
			if(*it == L'\n'){
				it++;
				custom_vector<short> line;
				line.push(0);
				itY++;
				model.metriks.push(line);
				continue;
			}
			ABC abc;
			GetCharABCWidthsW(hdc, *it, *it, &abc);
			cursorX += abc.abcA + abc.abcB + abc.abcC;
			model.metriks[itY].push(abc.abcA + abc.abcB + abc.abcC);
			it++;
		}
		return text_model();
	}
	void insert_to_model(HDC hdc, text_model &model, wchar_t a, int corsorX, int corsorY){
		ABC abc;
		GetCharABCWidthsW(hdc, a, a, &abc);
		model.metriks[corsorY].push(0);
		int char_size = abc.abcA + abc.abcB + abc.abcC;
		int line_size = model.metriks[corsorY].size;
		for(int i = corsorX; i < line_size; i++){
			int back = model.metriks[corsorY][i];
			model.metriks[corsorY][i] = char_size;
			char_size = back;
		}
	}
	void erase_to_model(HDC hdc, RECT rect, text_model &model, int corsorX, int corsorY){
		int line_size = model.metriks[corsorY].size;
		for(int i = corsorX; i < line_size - 1; i++) model.metriks[corsorY][i] = model.metriks[corsorY][i + 1];
		model.metriks[corsorY].pop();
	}
	int cursor_from_model(text_model &model, int X, int Y){
		int itY = min(model.text_height / Y, (int)model.metriks.size - 1);
		int cursor = 0;
		for(int i = 0; i < itY; i++){
			cursor += model.metriks[i].size;
		}
		if(X < 0)
			return cursor;
		for(short x : model.metriks[itY]){
			if(X < 0)
				return cursor;
			cursor++;
			X -= x;
		}
		return cursor;
	}
}
#include "Text.h"
#include "Main.h"

namespace Graphs{
	int Edit_Text::key_event(struct_key_event key, int virtual_key) { 
			if(text.size() && key.scan_code == 14 && key.is_first_message) text.pop_back();
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
			paint(hdc);
			return 0; 
		};

		int Edit_Text::char_event(struct_key_event key, wchar_t c) { 
			if(c != L'\b')
				text.push_back(c);
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
			paint(hdc);
			return 0; 
		};

		int Console_Text::mouse_wheel_event(int x, int y, short delta, int virtual_key) { 
			Graphics g(win->getDC());
			RectF rc;
			RectF layoutRect(0, 0, 10000, 10000);

			g.MeasureString(text.data(), -1, get_font(text_size, 1), layoutRect, &str_format, &rc);
			if(virtual_key & MK_SHIFT){
				if(delta > 0 && offset_x < 0)
					offset_x += 10;
				if(delta < 0 && -offset_x < rc.Width - abs_size.width)
					offset_x -= 10;
			}
			else{
				if(delta > 0 && offset_y < 0)
					offset_y += 10;
				if(delta < 0 && -offset_y < rc.Height - abs_size.height)
					offset_y -= 10;
			}
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
			paint(hdc);
			return 0; 
		};

		int Line_Text::resize_event() {
			View::resize_event();

			Size_ s = abs_size;
			abs_padding.reSize(s);
			realy_text_size = generate_text_size(win->hwnd, text, &str_format, text_size, s);
			realy_text_size = std::min(realy_text_size, text_size);
			return 0;
		};

		/*Size_ Console_Text::get_content_size(Size_ size) {
			Gdiplus::Graphics g(win->getDC());
			Gdiplus::RectF rc;
			//StringFormat stringFormat(StringFormat::GenericTypographic());
    	//stringFormat.SetFormatFlags(StringFormatFlagsLineLimit);

			RectF layoutRect(0, 0, 10000, 10000);

			g.MeasureString(text.data(), -1, get_font(text_size, 1), layoutRect, &str_format, &rc);
			size.width = rc.Width + 50;
			size.width.type = px;
			size.height.type = px;
			size.height = rc.Height + 50;
			return size;
		}*/
}
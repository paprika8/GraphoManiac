#include "Text.h"
#include "Main.h"

namespace Graphs{
	int Edit_Text::key_event(struct_key_event key, int virtual_key) { 
			text.pop_back();
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
			paint(hdc);
			return 0; 
		};

		int Edit_Text::char_event(struct_key_event key, wchar_t c) { 
			text += c;
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
			paint(hdc);
			return 0; 
		};

		Size_ Console_Text::get_content_size(Size_ size) {
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
		}
}
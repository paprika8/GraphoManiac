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
}
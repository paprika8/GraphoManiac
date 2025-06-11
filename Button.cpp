#include "Button.h"
#include "Main.h"

namespace Graphs {
	int Button::resize_event() {
			View::resize_event();

			Size_ s = abs_size;
			abs_padding.reSize(s);
			realy_text_size = generate_text_size(win->hwnd, text, &str_format, text_size, s);
			realy_text_size = std::min(realy_text_size, text_size);
			return 0;
		};
		int Button::mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) {
			if(button == L){
				switch (type)
				{
				case down:
				{
					is_down = 1;
					background.SetColor(down_colour);
					BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
					paint(hdc);
					capture(this);
				}
					break;
				case up:
					if(is_down){
						is_down = 0;
						background.SetColor(normal_colour);
						{
							BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
							paint(hdc);
						}
						re_capture(this);
						click(this);
					}
					break;
				
				default:
					break;
				}
			}
			return 0;
		};
}
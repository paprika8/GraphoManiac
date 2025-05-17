#include "Screen.h"
#include "Main.h"

namespace Graphs {

	int Screen::mouse_move_event(int x, int y, int virtual_key) {
		if(view_mouse_capture) return view_mouse_capture->mouse_move_event(x, y, virtual_key);
		return Composite::mouse_move_event(x, y, virtual_key);
	};
	int Screen::mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key)  {
		if(view_mouse_capture) return view_mouse_capture->mouse_event(button, type, x, y, virtual_key);
		return Composite::mouse_event(button, type, x, y, virtual_key);
	};
	int Screen::mouse_wheel_event(int x, int y, int delta, int virtual_key)  {
		if(view_mouse_capture) return view_mouse_capture->mouse_wheel_event(x, y, delta, virtual_key);
		return Composite::mouse_wheel_event(x, y, delta, virtual_key);
	};
	int Screen::key_event(struct_key_event key, int virtual_key)  {
		if(view_key_capture) return view_key_capture->key_event(key, virtual_key);
		return Composite::char_event(key, virtual_key);
	};
	int Screen::char_event(struct_key_event key, wchar_t c)  {
		if(view_key_capture) return view_key_capture->char_event(key, c);
		return Composite::char_event(key, c);
	};
	int Screen::paint_background_event(BufferHDC &hdc)  {
		hdc.graphic->FillRectangle(&background, abs_position.x, abs_position.y, abs_size.width, abs_size.height);
		return 0;
	};

	void Screen::capture(View* view)  {
		view_mouse_capture = view;
		SetCapture(win->hwnd);
	}
	void Screen::re_capture(View* view)  {
		view_mouse_capture = 0;
		ReleaseCapture();
	}
	void Screen::key_capture(View* view)  {
		view_key_capture = view;
	}
	void Screen::key_re_capture(View* view)  {
		view_key_capture = 0;
	}
}
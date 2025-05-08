#include "Views.h"
#include "positioner.h"

namespace Graphs
{

	int View::move(int X, int Y)
	{
		abs_position = Point_(X, Y);
		return move_event();
	}
	int View::paint(BufferHDC &hdc)
	{
		Gdiplus::Rect rc(abs_position.x, abs_position.y, abs_size.width, abs_size.height);
		hdc.graphic->SetClip(rc);
		paint_background_event(hdc);
		paint_event(hdc);
		return 0;
	}
	int View::resize(int width, int height)
	{
		abs_size = Size_(width, height);
		abs_padding = padding.toAbsolut(abs_size);
		return resize_event();
	}

	int Composite::move_event() {
		Positioning(this);
		return 0;
	};
	void Composite::add(View *value) {
		children.push_back(value);
		Positioning(this);
	}
	void Composite::insert(int index, View *value) {
		children.insert(children.begin() + index, value);
		Positioning(this);
	}

	void Composite::remove(View *value) {
		for(int i = 0; i < children.size(); i++)
			if(children[i] == value)
				remove(i);
	}

	void Composite::remove(int index) {
		delete children[index];
		children.erase(children.begin() + index);
		Positioning(this);
	}

	int Composite::resize_event()
	{
		Positioning(this);
		return 0;
	};
}
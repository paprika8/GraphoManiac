#pragma once

#include "graph.h"

namespace Graphs {
	struct GraphView : public View {
		graph gr;
		GraphView(View* aparent) : View(aparent) {
			gr.insert(node(1, 'a'));

			background.SetColor(Color(40,40,40));
		}
		int paint_event(BufferHDC &hdc) override {
			hdc.graphic->TranslateTransform(abs_position.x, abs_position.y);
			gr.draw(hdc);
			hdc.graphic->TranslateTransform(-abs_position.x, -abs_position.y);
			return 0;
		};
	};
}
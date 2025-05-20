#include "draw_graph.h"
#include "../tasks/functions.h"

namespace Graphs
{
	GraphView::GraphView(View* aparent) : View(aparent) {
		gr.insert(new node(1, 'a', &gr));
		lock_ids[1] = 1;

		*stop = 1;// 0 -stop    1-wait    2-run     3-complite

		std::thread tr = std::thread([this]() {
			int i = 0;
			double kf = 1;
			while (*stop) {
				int distance = std::min(abs_size.width.value, abs_size.height.value);
				if (distance < 400) {
					gr.node_radius = 30;
					gr.edge_width = 1;
					distance = 30;
				}
				else if (distance < 2000) {
					gr.node_radius = 50;
					gr.edge_width = 2.5;
					distance = 50;
				}
				else {
					gr.node_radius = 70;
					gr.edge_width = 3.5;
					distance = 70;
				}
				distance *= 3;
				if(is_enable_normalize){
					int min = gr.normalize(distance * kf);
					min /= kf;
					if (i > 50) {
						double bkf = (double)distance / min;
						if (kf / bkf > 1.1 || bkf / kf > 1.1)
							kf = bkf;
					}
				}
				if (i > 50) {
					i = 0;
					for (auto a : gr.nodes)
						a->mark = 'a';
					if(is_run)
						trigger(&gr);
					//comp_cnt(&gr);
				}
				{
					BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
					paint(hdc);
				}
				i++;
				Sleep(50);
			}
			*stop = 3;
		});

		tr.detach();

		background.SetColor(Color(40, 40, 180));

		aparent->key_capture(this);
	}

}


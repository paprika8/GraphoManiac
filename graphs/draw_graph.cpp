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
				if (is_enable_normalize) {
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
					if (is_run)
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
	int GraphView::paint_event(BufferHDC& hdc) {
		hdc.graphic->TranslateTransform(abs_position.x + offset_x, abs_position.y + offset_y);
		gr.draw(hdc);

		hdc.graphic->TranslateTransform(-offset_x, -offset_y);


		int radius = gr.node_radius;

		Gdiplus::Color col(80, 255, 255, 255);
		Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
		hdc.graphic->FillRectangle(&br, abs_size.width - radius - radius / 8, 0, radius + radius / 8, abs_size.height);
		br.SetColor(Color(40, 180, 40));
		hdc.graphic->FillEllipse(&br, abs_size.width - radius - radius / 16, radius / 16, radius, radius);
		if (moving_obj == mt_new_node)
			hdc.graphic->FillEllipse(&br, mouse_x + mouse_offset_x - radius / 2 + offset_x, mouse_y + mouse_offset_y - radius / 2 + offset_y, radius, radius);

		if (is_seting_answer)
			br.SetColor(Color(40, 180, 40));
		else
			br.SetColor(Color(180, 40, 40));
		hdc.graphic->FillRectangle(&br, abs_size.width - radius - radius / 16, radius / 8 + radius, radius, radius);

		if (is_enable_normalize)
			br.SetColor(Color(40, 180, 40));
		else
			br.SetColor(Color(180, 40, 40));
		hdc.graphic->FillRectangle(&br, abs_size.width - radius - radius / 16, radius / 16 + radius / 8 + 2 * radius, radius, radius);

		hdc.graphic->TranslateTransform(-abs_position.x, -abs_position.y);
		return 0;
	};
	int GraphView::mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) {
		if (button != mouse_buttons::L) {
			if (button != mouse_buttons::R)
				return 0;
			x -= abs_position.x;
			y -= abs_position.y;
			x -= offset_x;
			y -= offset_y;
			node* n = gr.find(x, y);
			if (n) {
				lock_ids[n->id] = 0;
				gr.erase(n);
				tmp_node = 0;
			}
			return 0;
		}

		int radius = gr.node_radius;

		x -= abs_position.x;
		y -= abs_position.y;

		if (type == click_event::down && virtual_key & MK_SHIFT) {
			moving_obj = mt_move;
			mouse_x = x;
			mouse_y = y;
		}
		else
			if (type == click_event::down && virtual_key & MK_CONTROL && !is_seting_answer) {
				x -= offset_x;
				y -= offset_y;
				node* _node = gr.find(x, y);
				if (!_node)
					return 0;
				if (moving_obj == mt_none) {
					moving_obj = mt_create_edge;
					tmp_node = _node;
					tmp_node->mark = 'b';
					BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
					paint(hdc);
				}
				else {
					for (auto ed : tmp_node->edges) {
						node* other_n;
						if (ed->point1 == tmp_node)
							other_n = ed->point2;
						else
							other_n = ed->point1;

						if (other_n == _node) {
							delete ed;
							moving_obj = mt_none;
							tmp_node->mark = 'a';
							BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
							paint(hdc);
							return 0;
						}
					}
					tmp_node->create_edge(_node);
					moving_obj = mt_none;
					tmp_node->mark = 'a';
					BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
					paint(hdc);
					return 0;
				}
			}
			else
				if (type == click_event::down) {
					if (tmp_node && moving_obj != mt_none) {
						moving_obj = mt_none;
						tmp_node->mark = 'a';
						BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
						paint(hdc);
					}
					if (x > abs_size.width - radius - radius / 8) {

						int n_b_x = abs_size.width - radius / 2 - radius / 16;
						int n_b_y = radius / 16 + radius / 2;

						n_b_x = x - n_b_x;
						n_b_y = y - n_b_y;

						if (n_b_x * n_b_x + n_b_y * n_b_y < radius * radius) {
							moving_obj = mt_new_node;
							mouse_offset_x = n_b_x;
							mouse_offset_y = n_b_y;
							return 0;
						}

						n_b_x = x;
						n_b_y = y;

						if (n_b_x > abs_size.width - radius - radius / 16 && n_b_y > radius / 8 + radius && n_b_x < abs_size.width - radius / 16 && n_b_y < radius / 8 + 2 * radius) {
							is_seting_answer = !is_seting_answer;
							if (is_seting_answer) {
								ans_task = 0;
								for (auto x : gr.nodes)
									x->mark = 'a';
							}
							else {
								for (auto x : gr.nodes)
									x->mark = 'a';
								task_button_trigger(&gr);
							}
							BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
							paint(hdc);
							return 0;
						}

						n_b_x = x;
						n_b_y = y - radius - radius / 16;
						if (n_b_x > abs_size.width - radius - radius / 16 && n_b_y > radius / 8 + radius && n_b_x < abs_size.width - radius / 16 && n_b_y < radius / 8 + 2 * radius) {
							is_enable_normalize = !is_enable_normalize;
							BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
							paint(hdc);
							return 0;
						}
					}
					else {
						x -= offset_x;
						y -= offset_y;
						node* n = gr.find(x, y);
						if (n) {
							if (is_seting_answer) {
								ans_task = ans_task ^ (1 << (n->id - 1));
								n->mark = 'a';
							}
							else
								moving_obj = mt_node;
							tmp_node = n;

						}
					}
				}
		if (type == click_event::up && moving_obj == mt_new_node &&
				x < abs_size.width - radius - radius / 8 && !is_seting_answer) {
			node* new_node = make_node(get_next_id(), 'a');
			lock_ids[new_node->id] = 1;
			x -= offset_x;
			y -= offset_y;
			new_node->x = x - radius / 2;
			new_node->y = y - radius / 2;
			gr.insert(new_node);
			moving_obj = mt_none;

			BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
			paint(hdc);
		}

		if (type == click_event::up && !is_seting_answer) {
			x -= offset_x;
			y -= offset_y;
			if (moving_obj != mt_create_edge)
				moving_obj = mt_none;
			else if (!gr.find(x, y) && moving_obj != mt_none) {
				moving_obj = mt_none;
				if (tmp_node) {
					tmp_node->mark = 'a';
					BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
					paint(hdc);
				}
			}
		}


		return 0;
	};

	int MassGraphView::mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) {
		if (button == mouse_buttons::R && virtual_key & MK_SHIFT) {
			if (type == click_event::down) {
				int xx = x - abs_position.x;
				int yy = y - abs_position.y;
				xx -= offset_x;
				yy -= offset_y;
				node* _n = gr.find(xx, yy);
				if (!_n)
					return 0;
				if (moving_obj != mt_set_mass) {
					tmp_node = _n;
					moving_obj = mt_set_mass;
				}
				else {
					for (auto ed : tmp_node->edges)
						if (ed->point1 == _n || ed->point2 == _n) {
							tmp_edge = ed;
							break;
						}
					return 0;
				}
			}
		}
		else
			return GraphView::mouse_event(button, type, x, y, virtual_key);
		return 0;
	};
	int MassGraphView::key_event(struct_key_event key, int virtual_key) {
		if (key.is_first_message && key.scan_code == 42) {
			mass = 0;
			is_shift = 1;
		}
		else
			if (key.transition_state && key.scan_code == 42) {
				is_shift = 1;
				if (tmp_edge) tmp_edge->value = mass;
				tmp_edge = 0;
				moving_obj = mt_none;
			}
		if (is_shift && key.is_first_message) {
			int code = key.scan_code;
			if (code == 11)
				code = 1;
			code--;
			if (code < 10) {
				mass *= 10;
				mass += code;
			}
		}
		return 0;
	};
}

//GraphView::
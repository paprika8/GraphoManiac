#pragma once

#include "consts.h"
#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"

namespace Graphs
{
	extern std::map<int, std::wstring> descriptions;
	struct nodeV : public node{

		nodeV(int i, char m, graph* agr) : node(i, m, agr) {};

		void draw(BufferHDC& hdc){
			int radius = gr->node_radius;

			Gdiplus::Color col(255, 255, 80);
			col = *colors[(char)(mark - 'a')];
			col = Gdiplus::Color(col.GetR() / 2, col.GetG() / 2, col.GetB() / 2);
			Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
			hdc.graphic->FillEllipse(&br, x, y, radius, radius);

			br.SetColor(*colors[(char)(mark - 'a')]);
			hdc.graphic->FillEllipse(&br, x + 2, y + 2, radius - 4, radius - 4);

			Gdiplus::StringFormat format;
			format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			br.SetColor(text_color);

			Gdiplus::RectF rc(x, y, radius, radius);

			hdc.graphic->DrawString(std::to_wstring(id).c_str(), -1, get_font(gr->text_size), rc, &format, &br);
		}
	};

	struct nodeChar : public node{

		nodeChar(int i, char m, graph* agr) : node(i, m, agr) {};

		void draw(BufferHDC& hdc){
			int radius = gr->node_radius;

			Gdiplus::Color col(255, 255, 80);
			col = *colors[0];
			col = Gdiplus::Color(col.GetR() / 2, col.GetG() / 2, col.GetB() / 2);
			Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
			hdc.graphic->FillEllipse(&br, x, y, radius, radius);

			br.SetColor(*colors[0]);
			hdc.graphic->FillEllipse(&br, x + 2, y + 2, radius - 4, radius - 4);

			Gdiplus::StringFormat format;
			format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			br.SetColor(text_color);

			Gdiplus::RectF rc(x, y, radius, radius);

			wchar_t wc = mark - 'a' + L'a';
			hdc.graphic->DrawString(&wc, 1, get_font(gr->text_size), rc, &format, &br);
		}
	};

	struct GraphMenuView : public View {
		graph name_gr;
		graph gr;
		int* stop = new int();
		bool is_run = false;
		std::vector<int> ans_ids;
		bool is_enable_normalize = 1;
		int text_size = 12;

		virtual node* make_node(int _id, char mark) {
			return new nodeV(_id, mark, &gr);
		}

		std::function<void(graph*)> trigger = [](graph*)->void {};
		std::function<void(graph*)> task_button_trigger = [](graph*)->void {};

		GraphMenuView(View* aparent);

		virtual ~GraphMenuView() {
			parent->key_re_capture(this);
			while (*stop != 3) {
				*stop = 0;
				Sleep(100);
			}
		}

		int mouse_move_event(int x, int y, int virtual_key) override {
			if (moving_obj == mt_node) {
				mouse_x = x - abs_position.x - offset_x;
				mouse_y = y - abs_position.y - offset_y;

				int dradius = gr.node_radius / 2;
				tmp_node->x = mouse_x - dradius;
				tmp_node->y = mouse_y - dradius;

				BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
				paint(hdc);
			}
			else if (moving_obj == mt_move) {
				x -= abs_position.x;
				y -= abs_position.y;
				offset_x += x - mouse_x;
				offset_y += y - mouse_y;
				mouse_x = x;
				mouse_y = y;

				BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
				paint(hdc);
			}
			else{
				x = x - abs_position.x - offset_x;
				y = y - abs_position.y - offset_y;
				node* n = gr.find(x, y);
				if (n) {
					moving_obj = mt_card;
					tmp_node = n;
				}
				else{
					tmp_node = 0;
				}
			}
			return 0;
		};

		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) override;

		int paint_event(BufferHDC& hdc) override;

		std::map<int, int> lock_ids;
	protected:
		enum moving_type {
			mt_none,
			mt_node,
			mt_move,
			mt_card
		};
		moving_type moving_obj = mt_none;
		int mouse_x = 0, mouse_y = 0;
		int mouse_offset_x = 0, mouse_offset_y = 0;
		int offset_x = 0, offset_y = 0;
		node* tmp_node = 0;

		int get_next_id() {
			int id = 1;
			while (lock_ids[id]) id++;
			lock_ids[id] = 1;
			return id;
		}
	};

	void create_ps();
}
#include "start_page.h"

#include "consts.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"
#include "page4.h"
#include "page5.h"
#include "page6.h"
#include "page7.h"
#include "page8.h"
#include "page9.h"
#include "page10_11.h"
#include "page12.h"

namespace Graphs
{

	std::map<int, std::wstring> descriptions = { {1, L"10"}, {2, L""}, {3, L""}, {4, L""}, {5, L""}, {6, L""},
	{7, L""}, {8, L""},{9, L""}, {10, L""}, {11, L""}, {12, L""}
	};

	GraphMenuView::GraphMenuView(View* aparent) : View(aparent) {

		*stop = 1;// 0 -stop    1-wait    2-run     3-complite

		gr.node_radius = 50;
		gr.edge_width = 2.5;
		name_gr.edge_width = 0;

		std::thread tr = std::thread
		(
			[this]() {
				int i = 0;
				double kf = 1;
				while (*stop) {
					int distance = std::min(abs_size.width.value, abs_size.height.value);
					if (distance < 400) {
						gr.node_radius = 50;
						gr.edge_width = 1;
						gr.text_size = 14;
						name_gr.node_radius = 80;
						name_gr.text_size = 50;
						text_size = 10;
						distance = 15;
					}
					else if (distance < 1000) {
						gr.node_radius = 70;
						gr.edge_width = 2.5;
						gr.text_size = 18;
						name_gr.node_radius = 80;
						name_gr.text_size = 60;
						text_size = 14;
						distance = 25;
					}
					else if (distance < 2000){
						gr.node_radius = 80;
						gr.edge_width = 3;
						gr.text_size = 20;
						name_gr.node_radius = 100;
						name_gr.text_size = 65;
						text_size = 18;
						distance = 35;
					}
					else{
						gr.node_radius = 90;
						gr.edge_width = 3.5;
						gr.text_size = 22;
						name_gr.node_radius = 120;
						name_gr.text_size = 70;
						text_size = 22;
						distance = 45;
					}
					distance *= 3;
					name_gr.normalize(name_gr.node_radius/ 1.3);
					if (is_enable_normalize) {
						int min = gr.normalize(distance * kf);
						min /= kf;
						if (i > 50) {
							double bkf = (double)distance / min;
							if (kf / bkf > 1.03 || bkf / kf > 1.03)
								kf = bkf;
						}
					}
					if (i > 50) {
						i = 0;
						if (is_run)
							trigger(&gr);
					}
					{
						BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
						paint(hdc);
					}
					i++;
					Sleep(50);
				}
				*stop = 3;

			}
		);

		int step = 90;
		int cursor = -80;
		
		

		char* name = "GraphoManiac";
		node* last = 0;
		for(char* c = name; *c; c++){
			node* nc = new nodeChar(0, *c, &name_gr);
			nc->x = cursor += step;
			nc->y = 0;
			name_gr.insert(nc);
			if(last)
				nc->create_edge(last);
			last = nc;
		}

		step = 80;
		int y_offset = win->screen->get_abs_size().height / 2 - step * 1.5;
		int x_offset = win->screen->get_abs_size().width / 2 - step * 2;

		node* n1 = make_node(get_next_id(), 'a');
		n1->x = x_offset + 0;
		n1->y = y_offset + 0;
		gr.insert(n1);
		node* n2 = make_node(get_next_id(), 'a');
		n2->x = x_offset + step;
		n2->y = y_offset + 0;
		gr.insert(n2);
		node* n3 = make_node(get_next_id(), 'a');
		n3->x = x_offset + step;
		n3->y = y_offset + step;
		gr.insert(n3);
		node* n4 = make_node(get_next_id(), 'a');
		n4->x = x_offset + 0;
		n4->y = y_offset + step;
		gr.insert(n4);
		node* n5 = make_node(get_next_id(), 'a');
		n5->x = x_offset + 0;
		n5->y = y_offset + step * 2;
		gr.insert(n5);
		node* n6 = make_node(get_next_id(), 'a');
		n6->x = x_offset + step;
		n6->y = y_offset + step * 2;
		gr.insert(n6);
		node* n7 = make_node(get_next_id(), 'a');
		n7->x = x_offset + step * 2;
		n7->y = y_offset + step * 2;
		gr.insert(n7);
		node* n8 = make_node(get_next_id(), 'a');
		n8->x = x_offset + step * 3;
		n8->y = y_offset + step * 2;
		gr.insert(n8);
		node* n9 = make_node(get_next_id(), 'a');
		n9->x = x_offset + step * 3;
		n9->y = y_offset + step;
		gr.insert(n9);
		node* n10 = make_node(get_next_id(), 'a');
		n10->x = x_offset + step * 2;
		n10->y = y_offset + step;
		gr.insert(n10);
		node* n11 = make_node(get_next_id(), 'a');
		n11->x = x_offset + step * 2;
		n11->y = y_offset + 0;
		gr.insert(n11);
		node* n12 = make_node(get_next_id(), 'a');
		n12->x = x_offset + step * 3;
		n12->y = y_offset + 0;
		gr.insert(n12);

		n1->create_edge(n2);
		n3->create_edge(n4);
		n2->create_edge(n11);
		n11->create_edge(n12);
		n12->create_edge(n9);
		n9->create_edge(n8);
		n8->create_edge(n7);
		n7->create_edge(n10);
		n7->create_edge(n6);
		n6->create_edge(n5);
		n5->create_edge(n4);

		tr.detach();

		background.SetColor(WHITE);

		aparent->key_capture(this);
	}
	int GraphMenuView::paint_event(BufferHDC& hdc) {
		hdc.graphic->TranslateTransform(abs_position.x + offset_x, abs_position.y + offset_y);
		gr.draw(hdc);
		name_gr.draw(hdc);

		if (tmp_node && moving_obj == mt_card) {
			Gdiplus::RectF rc = { 0, 0, 200, 100000 };
			Gdiplus::RectF res = { 0, 0, 200, 100000 };
			hdc.graphic->MeasureString(descriptions[tmp_node->id].c_str(), -1, get_font(text_size), rc, &res);
			Gdiplus::SolidBrush br = Gdiplus::SolidBrush(LIGHT);

			res.X = tmp_node->x + gr.node_radius;
			res.Y = tmp_node->y + gr.node_radius;
			res.Width += 20;
			res.Height += 20;

			hdc.graphic->FillRectangle(&br, res);

			res.X = res.X + 10;
			res.Y = res.Y + 10;
			res.Width -= 20;
			res.Height -= 20;

			br.SetColor(DARK);
			StringFormat format;
			hdc.graphic->DrawString(descriptions[tmp_node->id].c_str(), -1, get_font(text_size), res, &format, &br);
		}

		hdc.graphic->TranslateTransform(-offset_x, -offset_y);
		hdc.graphic->TranslateTransform(-abs_position.x, -abs_position.y);
		return 0;
	};
	int GraphMenuView::mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) {
		if (button != mouse_buttons::R) {
			if (button != mouse_buttons::L)
				return 0;
			x -= abs_position.x;
			y -= abs_position.y;
			x -= offset_x;
			y -= offset_y;
			node* n = gr.find(x, y);
			if (n) {
				switch (n->id)
				{
				case 1:
					create_p1();
					break;
				case 2:
					create_p2();
					break;
				case 3:
					create_p3();
					break;
				case 4:
					create_p4();
					break;
				case 5:
					create_p5();
					break;
				case 6:
					create_p6();
					break;
				case 7:
					create_p7();
					break;
				case 8:
					create_p8();
					break;
				case 9:
					create_p9();
					break;
				case 10:
				case 11:
					create_p10_11();
					break;
				case 12:
					create_p12();
					break;
				default:
					break;
				}
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
		else if (type == click_event::down) {
			parent->key_capture(this);
			if (tmp_node && moving_obj != mt_none) {
				moving_obj = mt_none;
				BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
				paint(hdc);
			}
			x -= offset_x;
			y -= offset_y;
			node* n = gr.find(x, y);
			if (n) {
				moving_obj = mt_node;
				tmp_node = n;
			}
			else{
				node* n = name_gr.find(x, y);
				if (n) {
					moving_obj = mt_node;
					tmp_node = n;
				}
			}	
		}

		if (type == click_event::up) {
			x -= offset_x;
			y -= offset_y;
			moving_obj = mt_none;
		}

		return 0;
	};
	void create_ps() {
		Composite* screen = dynamic_cast<Composite*>(win->screen);
		screen->background.SetColor(LIGHT);
		while (screen->children.size()) {
			delete screen->children.back();
		}

		//задаём выравнивание дочерних элементов по центру
		screen->margin.type |= MarginType::HCENTER | MarginType::VCENTER;

		Composite* b1 = new Composite(screen);
		b1->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		b1->margin = Margin(10, 10, 10, 10, MarginType::PARENT);
		b1->margin.type |= MarginType::PARENT;
		b1->is_vert_orientation = 1;
		b1->background.SetColor(WHITE);
		Composite* b2 = new Composite(b1);
		b2->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		b2->margin = Margin(5, 5, 5, 5, MarginType::PARENT);
		b2->margin.type |= MarginType::PARENT;
		b2->is_vert_orientation = 1;
		b2->background.SetColor(DARK);
		b1->add(b2);

		GraphMenuView* tx = new GraphMenuView(b2);
		tx->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		tx->margin = Margin(2, 2, 2, 2, MarginType::PARENT);
		tx->background.SetColor(WHITE);

		b2->add(tx);
		screen->add(b1);
	}
}
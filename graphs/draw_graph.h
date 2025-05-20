#pragma once

#include "graph.h"
#include "../Main.h"

#include <thread>
#include <map>

namespace Graphs {
	struct GraphView : public View {
		graph gr;
		int *stop = new int();
		bool is_run = false;
		unsigned short ans_task = 0;
		bool is_seting_answer = 0;
		bool is_enable_normalize = 0;

		virtual node* make_node(int _id, char mark){
			return new node(_id, mark, &gr);
		}

		std::function<void(graph*)> trigger = [](graph*)->void{};
		std::function<void(graph*)> task_button_trigger = [](graph*)->void{};

		GraphView(View* aparent);

		~GraphView(){
			parent->key_re_capture(this);
			while(*stop != 3){
				*stop = 0;
				Sleep(100);
			}
		}

		int mouse_move_event(int x, int y, int virtual_key) override { 
			if(moving_obj == mt_node || moving_obj == mt_create_edge || moving_obj == mt_new_node){
				mouse_x = x - abs_position.x - offset_x;
				mouse_y = y - abs_position.y - offset_y;

				if(moving_obj == mt_node){
					int dradius = gr.node_radius / 2;
					tmp_node->x = mouse_x - dradius;
					tmp_node->y = mouse_y - dradius;
				}

				BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
				paint(hdc);
			}
			if(moving_obj == mt_move){
				x -= abs_position.x;
				y -= abs_position.y;
				offset_x += x - mouse_x;
				offset_y += y - mouse_y;
				mouse_x = x;
				mouse_y = y;

				BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
				paint(hdc);
			}
			return 0; 
		};

		int key_event(struct_key_event key, int virtual_key) { 
			if(key.transition_state && key.scan_code == 29 && moving_obj == mt_create_edge){
				tmp_node->mark = 'a';
				moving_obj = mt_none;
				BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
				paint(hdc);
			}
			return 0;
		};


		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) override {
			if(button != mouse_buttons::L){
				if(button != mouse_buttons::R)
					return 0;
				x -= abs_position.x;
				y -= abs_position.y;
				x -= offset_x;
				y -= offset_y;
				node* n = gr.find(x, y);
				if(n){
					lock_ids[n->id] = 0;
					gr.erase(n);
					tmp_node = 0;
				}
				return 0;
			}
			
			int radius = gr.node_radius;

			x -= abs_position.x;
			y -= abs_position.y;

			if(type == click_event::down && virtual_key & MK_SHIFT){
				moving_obj = mt_move;
				mouse_x = x;
				mouse_y = y;
			}
			else
			if(type == click_event::down && virtual_key & MK_CONTROL && !is_seting_answer){
				x -= offset_x;
				y -= offset_y;
				node* _node = gr.find(x, y);
				if(!_node)
					return 0;
				if(moving_obj == mt_none){
					moving_obj = mt_create_edge;
					tmp_node = _node;
					tmp_node->mark = 'b';
					BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
					paint(hdc);
				}
				else{
					for(auto ed: tmp_node->edges){
						node* other_n;
						if(ed->point1 == tmp_node)
							other_n = ed->point2;
						else
							other_n = ed->point1;

						if(other_n == _node){
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
			if(type == click_event::down){
				if(tmp_node && moving_obj != mt_none){ 
					moving_obj = mt_none;
					tmp_node->mark = 'a';
					BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
					paint(hdc);
				}
				if(x > abs_size.width - radius - radius / 8){

					int n_b_x = abs_size.width - radius / 2  - radius / 16;
					int n_b_y = radius / 16 + radius / 2;

					n_b_x = x - n_b_x;
					n_b_y = y - n_b_y;

					if(n_b_x*n_b_x + n_b_y*n_b_y < radius*radius){
						moving_obj = mt_new_node;
						mouse_offset_x = n_b_x;
						mouse_offset_y = n_b_y;
						return 0;
					}

					n_b_x = x;
					n_b_y = y;

					if(n_b_x > abs_size.width - radius  - radius / 16 && n_b_y > radius / 8 + radius && n_b_x < abs_size.width  - radius / 16 && n_b_y < radius / 8 + 2 * radius ){
						is_seting_answer = !is_seting_answer;
						if(is_seting_answer){
							ans_task = 0;
							for(auto x: gr.nodes)
								x->mark = 'a';
						}
						else {
							for(auto x: gr.nodes)
								x->mark = 'a';
							task_button_trigger(&gr);
						}
						BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
						paint(hdc);
						return 0;
					}

					n_b_x = x;
					n_b_y = y - radius - radius / 16;
					if(n_b_x > abs_size.width - radius  - radius / 16 && n_b_y > radius / 8 + radius && n_b_x < abs_size.width  - radius / 16 && n_b_y < radius / 8 + 2 * radius ){
						is_enable_normalize = !is_enable_normalize;
						BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
						paint(hdc);
						return 0;
					}
				}
				else{
					x -= offset_x;
					y -= offset_y;
					node* n = gr.find(x, y);
					if(n){
						if(is_seting_answer){
							ans_task = ans_task ^ (1 << (n->id - 1));
							n->mark = 'a';
						}
						else
							moving_obj = mt_node;
						tmp_node = n;
						
					}
				}
			}
			if(type == click_event::up && moving_obj == mt_new_node && 
				x < abs_size.width - radius - radius / 8  && !is_seting_answer){
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

			if(type == click_event::up && !is_seting_answer){
				x -= offset_x;
				y -= offset_y;
				if(moving_obj != mt_create_edge)
					moving_obj = mt_none;
				else if(!gr.find(x, y) && moving_obj != mt_none){
					moving_obj = mt_none;
					if(tmp_node){ 
						tmp_node->mark = 'a';
						BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
						paint(hdc);
					}
				}
			}
				
	
			return 0;
		};

		int paint_event(BufferHDC &hdc) override {
			hdc.graphic->TranslateTransform(abs_position.x + offset_x, abs_position.y + offset_y);
			gr.draw(hdc);

			hdc.graphic->TranslateTransform(-offset_x, -offset_y);


			int radius = gr.node_radius;

			Gdiplus::Color col(80, 255, 255, 255);
			Gdiplus::SolidBrush br = Gdiplus::SolidBrush(col);
			hdc.graphic->FillRectangle(&br, abs_size.width - radius - radius / 8, 0, radius + radius / 8, abs_size.height);
			br.SetColor(Color(40,180,40));
			hdc.graphic->FillEllipse(&br, abs_size.width - radius  - radius / 16, radius / 16, radius, radius);
			if(moving_obj == mt_new_node)
				hdc.graphic->FillEllipse(&br, mouse_x + mouse_offset_x - radius / 2 + offset_x, mouse_y + mouse_offset_y - radius / 2 + offset_y, radius, radius);

			if(is_seting_answer)
				br.SetColor(Color(40,180,40));
			else
				br.SetColor(Color(180,40,40));
			hdc.graphic->FillRectangle(&br, abs_size.width - radius  - radius / 16, radius / 8 + radius, radius, radius);

			if(is_enable_normalize)
				br.SetColor(Color(40,180,40));
			else
				br.SetColor(Color(180,40,40));
			hdc.graphic->FillRectangle(&br, abs_size.width - radius  - radius / 16, radius / 16 + radius / 8 + 2*radius, radius, radius);

			hdc.graphic->TranslateTransform(-abs_position.x, -abs_position.y);
			return 0;
		};
	private:
		enum moving_type{
			mt_none,
			mt_node,
			mt_new_node,
			mt_create_edge,
			mt_move
		};
		moving_type moving_obj = mt_none;
		int mouse_x = 0, mouse_y = 0;
		int mouse_offset_x = 0, mouse_offset_y = 0;
		int offset_x = 0, offset_y = 0;
		node* tmp_node = 0;
		std::map<int, int> lock_ids;

		int get_next_id(){
			int id = 1;
			while(lock_ids[id]) id++;
			return id;
		}
	};
}
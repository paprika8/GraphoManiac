#pragma once

#include "graph.h"
#include "../Main.h"

#include <thread>

namespace Graphs {
	struct GraphView : public View {
		graph gr;
		int *stop = new int();

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
			if(button != mouse_buttons::L)
				return 0;
			
			int radius = gr.node_radius;

			x -= abs_position.x;
			y -= abs_position.y;

			if(type == click_event::down && virtual_key & MK_SHIFT){
				moving_obj = mt_move;
				mouse_x = x;
				mouse_y = y;
			}
			else
			if(type == click_event::down && virtual_key & MK_CONTROL){
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
				if(tmp_node){ 
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
				}
				else{
					x -= offset_x;
					y -= offset_y;
					node* n = gr.find(x, y);
					if(n){
						moving_obj = mt_node;
						tmp_node = n;
					}
				}
			}
			if(type == click_event::up && moving_obj == mt_new_node && 
				x < abs_size.width - radius - radius / 8){
				node* new_node = new node(id++, 'a', &gr);
				x -= offset_x;
				y -= offset_y;
				new_node->x = x - radius / 2;
				new_node->y = y - radius / 2;
				gr.insert(new_node);
				moving_obj = mt_none;

				BufferHDC hdc = BufferHDC(win->getDC(), win->size, this);
				paint(hdc);
			}

			if(type == click_event::up){
				x -= offset_x;
				y -= offset_y;
				if(moving_obj != mt_create_edge)
					moving_obj = mt_none;
				else if(!gr.find(x, y)){
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
		int id = 2;
		int mouse_x = 0, mouse_y = 0;
		int mouse_offset_x = 0, mouse_offset_y = 0;
		int offset_x = 0, offset_y = 0;
		node* tmp_node = 0;
	};
}
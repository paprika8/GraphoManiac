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
		std::vector<int> ans_ids;
		bool is_seting_answer = 0;
		bool is_enable_normalize = 0;

		virtual node* make_node(int _id, char mark){
			return new node(_id, mark, &gr);
		}

		std::function<void(graph*)> trigger = [](graph*)->void{};
		std::function<void(graph*)> task_button_trigger = [](graph*)->void{};

		GraphView(View* aparent);

		virtual ~GraphView(){
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


		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) override;

		int paint_event(BufferHDC &hdc) override;

		std::map<int, int> lock_ids;
	protected:
		enum moving_type{
			mt_none,
			mt_node,
			mt_new_node,
			mt_create_edge,
			mt_move,
			mt_set_mass
		};
		moving_type moving_obj = mt_none;
		int mouse_x = 0, mouse_y = 0;
		int mouse_offset_x = 0, mouse_offset_y = 0;
		int offset_x = 0, offset_y = 0;
		node* tmp_node = 0;
		

		int get_next_id(){
			int id = 1;
			while(lock_ids[id]) id++;
			return id;
		}
	};

	struct MassGraphView : public GraphView{
	public:
		edge* tmp_edge = 0;
		bool is_shift = 0;
		int mass = 0;

		MassGraphView(View* aparent) : GraphView(aparent) {
			//gr.erase(*gr.nodes.begin());
			//lock_ids[1] = 0;
		}


		int key_event(struct_key_event key, int virtual_key) override;

		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) override;

	};

	struct deikstraGraphView : public MassGraphView{
	public:
		edge* tmp_edge = 0;
		bool is_shift = 0;
		int mass = 0;

		node* make_node(int _id, char mark) override {
			return new deikstra_node(_id, mark, &gr);
		}

		deikstraGraphView(View* aparent) : MassGraphView(aparent) {}

	};
}
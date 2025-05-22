#pragma once

#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"
#include "positioner.h"

namespace Graphs{
	void create_p10_11(){
		Composite* screen = dynamic_cast<Composite*>(win->screen);

		while(screen->children.size()){
			delete screen->children.back();
		}

		//задаём выравнивание дочерних элементов по центру
		screen->margin.type |= MarginType::HCENTER | MarginType::VCENTER;


		Composite* comp = new Composite(screen);
		comp->margin.type = MarginType::LEFT | MarginType::VCENTER | MarginType::CONTENT;
		comp->is_vert_orientation = 1;

		Composite* butts = new Composite(comp);
		butts->margin.type = MarginType::TOP | MarginType::HCENTER | MarginType::CONTENT;
		butts->is_vert_orientation = 0;

		//создаём кнопку
		Button* bt1 = new Button(butts);
		bt1->size = Size_(pointUI(90), pointUI(1000, fr));
		bt1->margin = Margin(5,10,5,10);
		butts->add(bt1);

		Button* bt2 = new Button(butts);
		bt2->size = Size_(pointUI(90), pointUI(1000, fr));
		bt2->margin = Margin(5,10,5,10);
		butts->add(bt2);

		

		//создаём текстовый блок
		GraphView* tx = new GraphView(screen);
		tx->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		tx->margin = Margin(10,10,10,10, MarginType::PARENT);

		Edit_Text* ct = new Edit_Text(comp);
		ct->size = Size_(pointUI(200), pointUI(60));
		ct->margin = Margin(pointUI(5, percent),10,pointUI(5, percent),10, MarginType::CONTENT);
		comp->add(ct);
		comp->add(butts);

		bt1->click = [=](Button*)->void{
			std::wstring str = L"";
			std::vector<int> res = codding_Prufer(tx->gr);
			for(auto i: res)
				str += std::to_wstring(i) + L" ";
			ct->text = str;

			for(auto key: tx->lock_ids){
				key.second = 0;
			}

			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
			ct->paint(hdc);
		};
		bt2->click = [=](Button*)->void{
			std::wstring str = ct->text;

			std::vector<int> res;
			for(int i = 0; i < str.size(); i++){
				res.push_back(str[i] - '0');
			}
			decodding_Prufer(res, tx->gr);
			for(auto i: res)
				str += std::to_wstring(i);
			ct->text = L"";

			for(auto key: tx->gr.nodes){
				tx->lock_ids[key->id] = 1;
			}

			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
			ct->paint(hdc);
		};

		screen->add(comp);
		screen->add(tx);
	}
}
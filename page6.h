#pragma once

#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"

namespace Graphs{

	int parsing6(std::wstring str) {
		int res = 0;
		for(auto x: str){
			 if(x >= L'0' && x <= L'9'){
				res *= 10;
				res += x - L'0';
			 }
		}
		return res;
	}

	void create_p6(){
		Composite* screen = dynamic_cast<Composite*>(win->screen);

		while(screen->children.size()){
			delete screen->children.back();
		}

		//задаём выравнивание дочерних элементов по центру
		screen->margin.type |= MarginType::HCENTER | MarginType::VCENTER;

		Composite* comp = new Composite(screen);
		comp->margin.type = MarginType::LEFT | MarginType::VCENTER | MarginType::CONTENT;
		comp->is_vert_orientation = 1;

		//создаём кнопку
		Edit_Text* et = new Edit_Text(comp);
		et->size = Size_(pointUI(200), pointUI(60));
		et->margin = Margin(pointUI(5, percent),10,pointUI(5, percent),10, MarginType::CONTENT);
		comp->add(et);

		Line_Text* ct = new Line_Text(comp);
		ct->size = Size_(pointUI(200), pointUI(60));
		ct->margin = Margin(pointUI(5, percent),10,pointUI(5, percent),10, MarginType::CONTENT);
		comp->add(ct);

		Button* bt1 = new Button(comp);
		bt1->size = Size_(pointUI(90), pointUI(1000, fr));
		bt1->margin = Margin(pointUI(5, percent),10,pointUI(5, percent),10);
		comp->add(bt1);

		//создаём текстовый блок
		GraphView* tx = new GraphView(screen);
		tx->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		tx->margin = Margin(10,10,10,10, MarginType::PARENT);

		bt1->click = [=](Button*)->void{
			int cnt = comp_cnt(&tx->gr);
			int res = parsing6(et->text);
			if(cnt == res)
				ct->text = L"Верно";
			else
				ct->text = L"Неверно";
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
			ct->paint(hdc);
		};

		screen->add(comp);
		screen->add(tx);
	}
}
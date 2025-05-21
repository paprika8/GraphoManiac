#pragma once

#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "tasks/functions.h"

namespace Graphs{
	void create_p8(){
		Composite* screen = dynamic_cast<Composite*>(win->screen);

		while(screen->children.size()){
			delete screen->children.back();
		}

		//задаём выравнивание дочерних элементов по центру
		screen->margin.type |= MarginType::HCENTER | MarginType::VCENTER;

		//создаём кнопку
		Button* bt1 = new Button(screen);
		bt1->size = Size_(pointUI(90, percent), pointUI(1000, fr));
		bt1->margin = Margin(pointUI(5, percent),10,pointUI(5, percent),10);
		screen->add(bt1);

		//создаём текстовый блок
		deikstraGraphView* tx = new deikstraGraphView(screen);
		tx->size = Size_(pointUI(800, percent), pointUI(1000, percent));
		//tx->margin = Margin(10,10,10,10);

		bt1->click = [=](Button*)->void{
			deikstra(&tx->gr, tx->ans_ids);
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
		};

		screen->add(tx);
	}
}
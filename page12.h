#pragma once

#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"
#include "positioner.h"

namespace Graphs{
	void create_p12(){
		Composite* screen = dynamic_cast<Composite*>(win->screen);

		while(screen->children.size()){
			delete screen->children.back();
		}

		//задаём выравнивание дочерних элементов по центру
		screen->margin.type |= MarginType::HCENTER | MarginType::VCENTER;

		//создаём кнопку
		Button* bt1 = new Button(screen);
		bt1->size = Size_(pointUI(40, percent), pointUI(1000, fr));
		bt1->margin = Margin(pointUI(5, percent),10,pointUI(5, percent),10);
		screen->add(bt1);

		//создаём текстовый блок
		GraphView* tx = new GraphView(screen);
		tx->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		tx->margin = Margin(10,10,10,10, MarginType::PARENT);

		bt1->click = [=](Button*)->void{
			accept_12(&tx->gr);
			Positioning(screen);
			BufferHDC hdc = BufferHDC(win->getDC(), win->size, tx);
			tx->paint(hdc);
		};

		screen->add(tx);
	}
}
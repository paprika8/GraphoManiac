#pragma once

#include "graphs/graph.h"
#include "graphs/draw_graph.h"
#include "Button.h"
#include "Text.h"
#include "tasks/functions.h"
#include "positioner.h"

namespace Graphs{
	void create_p9(){
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
		deikstraGraphView* tx = new deikstraGraphView(screen);
		tx->size = Size_(pointUI(400, percent), pointUI(1000, percent));
		//tx->margin = Margin(10,10,10,10);

		Console_Text* ct = new Console_Text(screen);
		ct->size = Size_(pointUI(90, percent), pointUI(1000, fr));
		ct->margin = Margin(pointUI(5, percent),10,pointUI(5, percent),10, MarginType::CONTENT);
		screen->add(ct);

		bt1->click = [=](Button*)->void{
			std::wstring str = accept_9(&tx->gr);
			ct->text = str;
			Positioning(screen);
		};

		screen->add(tx);
	}
}
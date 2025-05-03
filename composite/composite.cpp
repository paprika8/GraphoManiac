#include "composite.h"
#include "../positioner.h"


namespace BoolApp{
void Composite::add(View *aview)
	{
		children.push(aview);
		aview->parent = this;
		if(enabled){
			aview->Construct();
			((PComposite*)PV)->add(aview->PV);
		}
	}
	void PComposite::add(ProcessView *apview)
	{
		children.push(apview);
		apview->parent = this;
		Positioning(this);
		InvalidateRect(hwnd, 0, 1);
		for(auto el: children)
			InvalidateRect(el->hwnd, 0, 1);
	}
}
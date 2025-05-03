#include "component.h"
#include "../positioner.h"


namespace BoolApp{
	void PComponent::add(ProcessView *apview)
	{
		delete child;
		child = apview;
		apview->parent = this;
		Positioning(this);
		//InvalidateRect(hwnd, 0, 1);
		//InvalidateRect(apview->hwnd, 0, 1);
	}
	void Component::add(View *aview)
	{
		delete child;
		child = aview;
		aview->parent = this;
		if(enabled){
			aview->Construct();
			((PComponent*)PV)->add(aview->PV);
		}
	}
}
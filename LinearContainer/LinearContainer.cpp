#include "LinearContainer.h"

namespace BoolApp
{
	void PLContainer::construction()
	{
		hwnd = CreateWindowEx(
			WS_EX_TRANSPARENT,
			view->getSzWindowClass().c_str(),
			L"",
			WS_VISIBLE | WS_CHILD,
			0, 0, 100, 100,
			hwnd,
			0,
			instance,
			0
		);
		SetWindowLongPtr(hwnd, 0, (LONG_PTR) this);
	}
	Size PLContainer::GetContentSize(Size size)
	{
		if(!(margin.type & MarginType::CONTENT))
			return this->size;
		Size res;
		Size asize;
		Margin amargin ( 0 , 0 , 0 , 0 );
		for ( int i = 0; i < len (); i++ ) {
			asize = getPView ( i )->GetContentSize(getPView ( i )->size).toAbsolut ( size );
			amargin = getPView ( i )->margin.toAbsolut ( size );
			if ( !is_vert_orientation )
				res = res.plusRight ( asize, amargin );
			else
				res = res.plusBottom ( asize , amargin );
		}
		return res;
	}
	LRESULT LinearContainer::wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam, ProcessView *ptr)
	{
		return DefWindowProc(ahwnd, message, wparam, lparam);
	}
}
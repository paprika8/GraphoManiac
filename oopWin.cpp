#include "oopWin.h"
#include "positioner.h"

namespace Graphs
{

	template <class T>
	void deleteSafely(T *&ptr)
	{
		T *buff = ptr;
		ptr = 0;
		delete buff;
	}
	void paint ( HWND hwnd )//зомбикод
	{
		PAINTSTRUCT pPaintStruct;
		HDC hdc = BeginPaint ( hwnd , &pPaintStruct );
		int cx = pPaintStruct.rcPaint.right - pPaintStruct.rcPaint.left;
		int cy = pPaintStruct.rcPaint.bottom - pPaintStruct.rcPaint.top;
		HDC hMemDC;
		HBITMAP hBmp;
		HBITMAP hOldBmp;
		POINT ptOldOrigin;

		ProcessView* pv = GetWindowLongPtr ( hwnd , 0 );

		// Create new bitmap-back device context, large as the dirty rectangle.
		hMemDC = CreateCompatibleDC ( pPaintStruct.hdc );
		hBmp = CreateCompatibleBitmap ( pPaintStruct.hdc , cx , cy );
		hOldBmp = ( HBITMAP ) SelectObject ( hMemDC , hBmp );

		// Do the painting into the memory bitmap.
		OffsetViewportOrgEx ( hMemDC , -( pPaintStruct.rcPaint.left ) ,
			-( pPaintStruct.rcPaint.top ) , &ptOldOrigin );

		RECT rcPs = pPaintStruct.rcPaint;

		//pv->view->paint ( hMemDC , pPaintStruct );

		SetViewportOrgEx ( hMemDC , ptOldOrigin.x , ptOldOrigin.y , NULL );

		// Blit the bitmap into the screen. This is really fast operation and although
		   // the CustomPaint() can be complex and slow there will be no flicker any more.
		BitBlt ( pPaintStruct.hdc , pPaintStruct.rcPaint.left , pPaintStruct.rcPaint.top ,
			cx , cy , hMemDC , 0 , 0 , SRCCOPY );

		// Clean up.
		SelectObject ( hMemDC , hOldBmp );
		DeleteObject ( hBmp );
		DeleteDC ( hMemDC );
		EndPaint(hwnd, &pPaintStruct);
	}

	HINSTANCE instance = 0;
	std::set<std::wstring> isRegistry = std::set<std::wstring>();

	void init(HINSTANCE ainstance)
	{
		instance = ainstance;
	}

	LRESULT wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case WM_ERASEBKGND:
		/*if(GetWindowLongPtr(ahwnd, 0)){
			ProcessView *ptr = GetWindowLongPtr(ahwnd, 0);
			if(!ptr->view)
				return 1;
			PAINTSTRUCT pstruct;
			HDC hdc = BeginPaint ( ahwnd , &pstruct );
			Gdiplus::Graphics g(hdc);
			auto rcDirty = pstruct.rcPaint;
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush ( ptr->view->background );
			g.FillRectangle ( brush , rcDirty.left , rcDirty.top , ( int ) ( rcDirty.right - rcDirty.left ) , ( int ) ( rcDirty.bottom - rcDirty.top ) );
			delete brush;
			EndPaint(ahwnd, &pstruct);
		}*/
    		return 1;
		case WM_PAINT:
		{
			ProcessView *ptr = GetWindowLongPtr(ahwnd, 0);
			ptr->view->paint(ahwnd);
			return 0;
		}
		case WM_NCCREATE:
		{
			SetWindowLongPtr(ahwnd, 0, 0);
			return DefWindowProc(ahwnd, message, wparam, lparam);
		}
		case WM_NCDESTROY:
		{ // конец удаления
			HWND parentHWND = GetParent(ahwnd);
			if (parentHWND)
			{
				InvalidateRect(parentHWND, 0, 0);
			}
		}
		case WM_DESTROY:
		{ // начало удаления
			ProcessView *ptr = GetWindowLongPtr(ahwnd, 0);
			if (ptr)
			{
				SetWindowLongPtr(ahwnd, 0, 0);
				ptr->hwnd = 0;
				delete ptr;
			}
			return DefWindowProc(ahwnd, message, wparam, lparam);
		}
		case WM_SIZE:
		{
			ProcessView *ptr = GetWindowLongPtr(ahwnd, 0);
			if (!ptr)
			{
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			if (GetWindowLong(ahwnd, GWL_STYLE) & WS_SIZEBOX)
			{
				ptr->size.height = HIWORD(lparam);
				ptr->size.width = LOWORD(lparam);
			}
			ptr->view->resize(ptr, ptr->point, Size(LOWORD(lparam), HIWORD(lparam)));
		}
		case WM_MOVE:
		{
			ProcessView *ptr = GetWindowLongPtr(ahwnd, 0);
			if (!ptr)
			{
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			Positioning(ptr);
			ptr->view->resize(ptr, Point(LOWORD(lparam), HIWORD(lparam)), ptr->getAbsoluteSize());
			return ptr->view->wndProc(ahwnd, message, wparam, lparam, ptr);
		}
		default:
		{
			ProcessView *ptr = GetWindowLongPtr(ahwnd, 0);
			if (!ptr)
			{
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			return ptr->view->wndProc(ahwnd, message, wparam, lparam, ptr);
		}
		}
		return DefWindowProc(ahwnd, message, wparam, lparam);
	}

	int run()
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}

	void View::Construct()
	{
		if (PV != 0)
		{
			return;
		}
		if(parent)
			PV = VConstruct(parent->PV);
		else
			PV = VConstruct(0);
		builder->build(PV);
		Positioning(PV);
		enabled = true;
	}

	void View::enable()
	{
		if (enabled)
		{
			return;
		}
		enabled = true;
		Construct();
	}

	void View::disable()
	{
		if (!enabled)
		{
			return;
		}
		enabled = false;
		if(PV)
			deleteSafely(PV);
	}

	ProcessView::ProcessView(HWND ahwnd, View *aview)
	{
		hwnd = ahwnd;
		view = aview;

		if (!isRegistry.contains(aview->getSzWindowClass()))
		{
			aview->sregister();
			isRegistry.insert(aview->getSzWindowClass());
		}
		if (aview->parent)
		{
			parent = aview->parent->PV;
		}
	}

	ProcessView::~ProcessView()
	{
		if (view)
		{
			view->PV = 0;
			view->disable();
			view = 0;
		}

		if (hwnd)
		{
			SetWindowLongPtr(hwnd, 0, 0);
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}
	void SizeBuilder::build(ProcessView *apv)
	{
		apv->size = size;
		apv->margin = margin;
		apv->padding = padding;
		Positioning(apv);
	}
}
template<typename T>
T max(T a, T b){
	if(a > b)
	return a;
	return b;
}
template<typename T>
T min(T a, T b){
	if(a < b)
	return a;
	return b;
}
Gdiplus::Color operator-( Gdiplus::Color start , Gdiplus::Color DeltaColor ) {
	Gdiplus::Color finish = Gdiplus::Color ( start.GetA () , max(0, start.GetR () - DeltaColor.GetR ()) , max(0, start.GetG () - DeltaColor.GetG ()) , max(0, start.GetB () - DeltaColor.GetB ()) );
	return finish;
}

Gdiplus::Color operator+( Gdiplus::Color start , Gdiplus::Color DeltaColor ) {
	Gdiplus::Color finish = Gdiplus::Color ( start.GetA () , min(255, start.GetR () + DeltaColor.GetR ()) , min(255, start.GetG () + DeltaColor.GetG ()) , min(255, start.GetB () + DeltaColor.GetB ()) );
	return finish;
}

Gdiplus::Color operator-( Gdiplus::Color start , int Delta ) {
	Gdiplus::Color finish = Gdiplus::Color ( start.GetA () , max(0, start.GetR () - Delta) , max(0, start.GetG () - Delta) , max(0, start.GetB () - Delta) );
	return finish;
}

Gdiplus::Color operator+( Gdiplus::Color start , int Delta ) {
	Gdiplus::Color finish = Gdiplus::Color ( start.GetA () , min(255, start.GetR () + Delta) , min(255, start.GetG () + Delta) , min(255, start.GetB () + Delta));
	return finish;
}
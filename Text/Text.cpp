#include "Text.h"

namespace BoolApp {
	void PScrollText::construction()
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
	void ScrollText::paint(HWND hwnd)
	{
		PAINTSTRUCT pstruct;
		HDC hdc = BeginPaint ( hwnd , &pstruct );
		PScrollText* f = PV;
		Gdiplus::Graphics g ( hdc );
		LPRECT rcDirty = &pstruct.rcPaint;
		Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush ( background );
		g.FillRectangle ( brush , rcDirty->left , rcDirty->top , ( int ) ( rcDirty->right - rcDirty->left ) , ( int ) ( rcDirty->bottom - rcDirty->top ) );
		delete brush;

		Size AbsSize = f->getAbsoluteSize ();
		f->padding.toAbsolut(AbsSize).reSize ( AbsSize );
		if ( AbsSize.height < f->TextHeight ) {
			//Фон ползунка
			brush = new Gdiplus::SolidBrush ( background - 18 );
			Gdiplus::Rect ScrollRect = Gdiplus::Rect ( rcDirty->right - f->padding.right , rcDirty->top , 20 , rcDirty->bottom - rcDirty->top );
			g.FillRectangle ( brush , ScrollRect );
			delete brush;

			//ползунок
			brush = new Gdiplus::SolidBrush ( background - 38 );
			int c = std::max ( f->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
			Gdiplus::Rect ScrollBar = Gdiplus::Rect ( rcDirty->right - f->padding.right + 2 , rcDirty->top + ( ( double ) f->WPos ) / f->MaxTextHeight * ( f->getAbsoluteSize ().height - c ) , 16 , c );
			g.FillRectangle ( brush , ScrollBar );
			delete brush;
		}

		RECT r = RECT ( *rcDirty );
		r.top = -f->WPos;
		f->padding.toAbsolut(f->getAbsoluteSize()).reRect ( r );
		brush = new Gdiplus::SolidBrush (text_color);
		g.DrawString ( text.c_str () , -1 , font , Gdiplus::RectF ( r.left , r.top , r.right - r.left , r.bottom - r.top ) , stringFormat , brush );
		delete brush;
		EndPaint(hwnd, &pstruct);
	}
	LRESULT ScrollText::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, ProcessView *pData)
	{
		PScrollText* f = dynamic_cast < PScrollText* > ( pData );
		switch ( uMsg )
		{
		case WM_ERASEBKGND:
		{
			return 1;
		}
		case WM_PAINT:
		{
			this->View::paint(hwnd);
			break;
		}
		
		case WM_LBUTTONDOWN:
		{
			Size AbsSize = pData->getAbsoluteSize ();
			int width = AbsSize.width;
			Padding padding = pData->padding;
			padding.right += 30;
			padding.left += 5;
			padding.reSize ( AbsSize );
			
			int c = 0; 
			if(f->TextHeight)
				c = std::max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
			int a = pData->getAbsoluteSize ().height;
			int pos = pData->padding.top + ( ( double ) f->WPos ) / f->MaxTextHeight * ( pData->getAbsoluteSize ().height - c );
			int xPos = LOWORD ( lParam );
			int yPos = HIWORD ( lParam );
			if ( pos < yPos && yPos < pos + c && xPos > width-pData->padding.right && xPos < width-2) {
				f->isDown = 1;
				SetCapture ( hwnd );
				f->oldY = yPos;
			}
			

		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( f->isDown ) {
				Size AbsSize = pData->getAbsoluteSize ();
				pData->padding.reSize ( AbsSize );
				int c = std::max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
				int a = pData->getAbsoluteSize ().height;
				short yPos = HIWORD ( lParam );

				int deltaPos = yPos - f->oldY;
				f->WPos += deltaPos * f->MaxTextHeight / ( a - c );
				if ( f->WPos > f->MaxTextHeight )
				{
					f->WPos = f->MaxTextHeight;
				}

				if ( f->WPos < 0 )
				{
					f->WPos = 0;
				}
				f->oldY = yPos;
				InvalidateRect ( hwnd , 0 , 0 );
			}


		}
		break;
		case WM_MOUSELEAVE:
		case WM_LBUTTONUP:
		{
			if( f->isDown )
				ReleaseCapture ();
			f->isDown = 0;
		}
		break;

		case WM_MOUSEWHEEL:
		{
			int WPDelta = GET_WHEEL_DELTA_WPARAM ( wParam );
			WPDelta /= 15;

			f->WPos -= WPDelta;
			if ( f->WPos > f->MaxTextHeight )
			{
				f->WPos = f->MaxTextHeight;
			}
			
			if ( f->WPos < 0 )
			{
				f->WPos = 0;
			}
			InvalidateRect ( hwnd, 0, 0 );
			return 0;
		}
		case WM_SIZE:
		case WM_SETSCROLLEDTEXT:
		{
			Size buff = f->getAbsoluteSize();
			f->padding.reSize ( buff );
			Gdiplus::RectF rect1 ( 0 , 0 , buff.width , 0 );
			Gdiplus::RectF rect2;
			Gdiplus::Graphics graph ( GetDC ( 0 ) );
			graph.MeasureString ( text.c_str () , -1 , font , rect1 , stringFormat , &rect2 , 0 , 0 );
			f->MaxTextHeight = rect2.Height - buff.height;
			f->TextHeight = rect2.Height;
			if(f->MaxTextHeight > 0) f->padding.right = 20;
			else f->padding.right = 0;
			if ( f->WPos > f->MaxTextHeight )
			{
				f->WPos = f->MaxTextHeight;
			}
			if ( f->WPos < 0 )
			{
				f->WPos = 0;
			}
			InvalidateRect ( hwnd, 0, 0 );
			return 0;
		}
		default:
		{
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		}
		}
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}
	Gdiplus::Font *createFont(int size)
	{
		//Gdiplus::FontFamily family ( L"Times New Roman" );
		Gdiplus::FontFamily family ( L"Consolas" );
		return new Gdiplus::Font ( &family , size , Gdiplus::FontStyleRegular , Gdiplus::UnitPixel );
	}


}
#include "Edit.h"
//#include <time.h>

namespace BoolApp {
	void PEdit::construction()
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
	void Edit::drawText(HDC hdc, Gdiplus::Rect rect){
		SetBkMode(hdc, TRANSPARENT);
		wchar_t* it = text.data;
		int row = rect.Y, cursor = rect.X;
		TEXTMETRIC tm;
		int cursorX = 0, cursorY = 0;;
		GetTextMetrics(hdc, &tm);
		int stepY = tm.tmHeight;
		while(*it){
			if(*it == L'\n'){
				it++;
				cursor = rect.X;
				row += stepY + 10;
				continue;
			}
			ABC abc;
			GetCharABCWidthsW(hdc, *it, *it, &abc);
			cursor += abc.abcA;
			if(cursor + abc.abcB > rect.X + rect.Width){
				cursor = rect.X + abc.abcA;
				row += stepY + 10;
				if(row > rect.Y + rect.Height)
					break;
			}
			RECT r;
			r.top = row;
			r.left = cursor;
			r.right = rect.Width + rect.X;
			r.bottom = rect.Y + rect.Height;
			DrawTextW(hdc, it, 1, &r, DT_NOPREFIX);
			cursor += abc.abcB;
			if(it - text.data == text_cursor+untext_cursor)
			{
				cursorX = cursor - abc.abcB - abc.abcA;
				cursorY = row;
			}
			cursor += abc.abcC;
			it++;
		}
		if(it - text.data == text_cursor+untext_cursor)
		{
			cursorX = cursor;
			cursorY = row;
		}
		//if(clock() % 2){//мигание каретки
		MoveToEx(hdc, cursorX, cursorY, NULL);
		LineTo(hdc, cursorX, cursorY + stepY);
		//}
	}
	void Edit::rect_text(HDC hdc, Gdiplus::Rect rect, Gdiplus::Rect &res){
		wchar_t* it = text.data;
		int row = rect.Y, cursor = rect.X;
		SelectObject(hdc, font);
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		int stepY = tm.tmHeight;
		while(*it){
			ABC abc;
			GetCharABCWidthsW(hdc, *it, *it, &abc);
			cursor += abc.abcA;
			if(cursor + abc.abcB > rect.X + rect.Width){
				cursor = rect.X + abc.abcA;
				row += stepY + 10;
				//if(row > rect.Y + rect.Height)
				//	return;
			}
			cursor += abc.abcB;
			/*if(it - text.data == text_cursor + untext_cursor)
			{
				MoveToEx(hdc, cursor, row, NULL);
				LineTo(hdc, cursor, row + stepY);
			}*/
			cursor += abc.abcC;
			it++;
		}
		row += stepY + 10;
		res.Height = row - rect.Y;
		res.Width = rect.Width;
		res.Y = rect.Y;
		res.X = rect.X;

	}
	void Edit::paint(HWND hwnd)
	{
		PAINTSTRUCT pstruct;
		HDC hdc = BeginPaint ( hwnd , &pstruct );
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(text_color.GetR(), text_color.GetG(), text_color.GetB()));
		SelectObject(hdc, font);
		PEdit* f = PV;
		Gdiplus::Graphics g ( hdc );
		LPRECT rcDirty = &pstruct.rcPaint;
		Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush ( background );
		g.FillRectangle ( brush , rcDirty->left , rcDirty->top , ( int ) ( rcDirty->right - rcDirty->left ) , ( int ) ( rcDirty->bottom - rcDirty->top ) );
		delete brush;


		Size AbsSize = f->getAbsoluteSize ();
		f->padding.reSize ( AbsSize );
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
		f->padding.reRect ( r );
		drawText(hdc, Gdiplus::Rect ( r.left , r.top , r.right - r.left , r.bottom - r.top ));
		EndPaint(hwnd, &pstruct);
	}
	LRESULT Edit::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, ProcessView *pData)
	{
		PEdit* f = dynamic_cast < PEdit* > ( pData );
		switch ( uMsg )
		{
		case WM_PAINT:
		{
			this->View::paint(hwnd);
			break;
		}
		break;
		
		case WM_LBUTTONDOWN:
		{
			Size AbsSize = pData->getAbsoluteSize ();
			int width = AbsSize.width;
			Padding padding = pData->padding;
			padding.right += 30;
			padding.left += 5;
			padding.reSize ( AbsSize );
			int c = std::max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
			int a = pData->getAbsoluteSize ().height;
			int pos = pData->padding.top + ( ( double ) f->WPos ) / f->MaxTextHeight * ( pData->getAbsoluteSize ().height - c );
			int xPos = LOWORD ( lParam );
			int yPos = HIWORD ( lParam );
			if ( pos < yPos && yPos < pos + c && xPos > width-18 && xPos < width-2) {
				f->isDown = 1;
				SetCapture ( hwnd );
				f->oldY = yPos;
			}
			SetFocus(hwnd);
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
			Gdiplus::Rect rect1 ( 0 , 0 , buff.width , 0 );
			Gdiplus::Rect rect2;
			Gdiplus::Graphics graph ( GetDC ( 0 ) );
			//graph.MeasureString ( text.c_str () , -1 , font , rect1 , stringFormat , &rect2 , 0 , 0 );
			rect_text(GetDC(hwnd), rect1, rect2);
			f->MaxTextHeight = rect2.Height - buff.height;
			f->TextHeight = rect2.Height;
			if(f->MaxTextHeight > 0) f->padding.right = 20;
			else f->padding.right = 0;
			if ( f->WPos > f->MaxTextHeight )
			{
				//f->padding.right = 20;
				f->WPos = f->MaxTextHeight;
			}
			if ( f->WPos < 0 )
			{
				//f->padding.right = 0;
				f->WPos = 0;
			}
			InvalidateRect(hwnd, 0, 1);
			return 0;
		}
		case WM_CHAR:{
			wchar_t ch = wParam;
			if(ch == L'\b')
				return 0;
			text.add(ch, text_cursor + untext_cursor);
			text_cursor++;
			SendMessage ( PV->hwnd , WM_SETSCROLLEDTEXT , 0 , 0 );
			return 0;
		}
		case WM_KEYDOWN: {
            // Обрабатываем нажатия стрелочек
            switch (wParam) {
                case VK_RIGHT:
					if(text_cursor + untext_cursor != text.size)
						text_cursor++;
					InvalidateRect(hwnd, 0, 1);
                    break;
                case VK_LEFT:
					if(text_cursor != 0)
						text_cursor--;
					InvalidateRect(hwnd, 0, 1);
                    break;
				case VK_BACK:
					if(text_cursor)
						text.del(text_cursor + untext_cursor);
					if(text_cursor)
						text_cursor--;
					SendMessage ( PV->hwnd , WM_SETSCROLLEDTEXT , 0 , 0 );
                    break;
				case VK_RETURN:
					/*wchar_t c = L'\n';
					text.add(c, text_cursor + untext_cursor);
					text_cursor++;
					SendMessage ( PV->hwnd , WM_SETSCROLLEDTEXT , 0 , 0 );*/
					enter(this);
                    break;
            }
			InvalidateRect(hwnd, 0, 1);
            return 0;
        }
		default:
		{
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		}
		}
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}
}
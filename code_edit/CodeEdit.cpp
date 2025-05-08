#include "CodeEdit.h"
//#include <time.h>

namespace Graphs {
	void PCodeEdit::construction()
	{
		int i = GetLastError();
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
		i = GetLastError();
		SetWindowLongPtr(hwnd, 0, (LONG_PTR) this);
	}
	void CodeEdit::drawText(HDC hdc, Gdiplus::Rect rect){
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
			if(it - text.data == text_cursor)
			{
				cursorX = cursor - abc.abcB - abc.abcA;
				cursorY = row;
			}
			cursor += abc.abcC;
			it++;
		}
		if(it - text.data == text_cursor)
		{
			cursorX = cursor;
			cursorY = row;
		}
		//if(clock() % 2){//мигание каретки
		MoveToEx(hdc, cursorX, cursorY, NULL);
		LineTo(hdc, cursorX, cursorY + stepY);
		//}
	}
	void CodeEdit::paint(HWND hwnd)//TODO но вроде сделал
	{
		PAINTSTRUCT pstruct;
		HDC hdc = BeginPaint ( hwnd , &pstruct );
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(text_color.GetR(), text_color.GetG(), text_color.GetB()));
		SelectObject(hdc, font);
		PCodeEdit* f = PV;
		Gdiplus::Graphics g ( hdc );
		LPRECT rcDirty = &pstruct.rcPaint;
		Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush ( background );
		g.FillRectangle ( brush , rcDirty->left , rcDirty->top , ( int ) ( rcDirty->right - rcDirty->left + 1 ) , ( int ) ( rcDirty->bottom - rcDirty->top + 1 ) );
		delete brush;


		Size AbsSize = f->getAbsoluteSize ();
		Padding padding = f->padding.toAbsolut(AbsSize);
		padding.reSize (AbsSize);

		if ( AbsSize.width < f->TextWidth && f->TextWidth ) {
			//Фон ползунка
			brush = new Gdiplus::SolidBrush ( background - 18 );
			Gdiplus::Rect ScrollRect = Gdiplus::Rect ( rcDirty->left , rcDirty->bottom - f->padding.bottom , rcDirty->right - rcDirty->left + 1 - 20 , 20 );
			g.FillRectangle ( brush , ScrollRect );
			delete brush;

			//ползунок
			brush = new Gdiplus::SolidBrush ( background - 38 );
			int c = std::max ( (f->getAbsoluteSize ().width - 20) * AbsSize.width / f->TextWidth , 20 );
			Gdiplus::Rect ScrollBar = Gdiplus::Rect ( rcDirty->left + ( ( double ) f->HPos ) / f->MaxTextWidth * ( f->getAbsoluteSize ().width - c ), rcDirty->bottom - f->padding.bottom + 2 , c , 16 );
			g.FillRectangle ( brush , ScrollBar );
			delete brush;
		}

		if ( AbsSize.height < f->TextHeight && f->TextHeight ) {
			//Фон ползунка
			brush = new Gdiplus::SolidBrush ( background - 18 );
			Gdiplus::Rect ScrollRect = Gdiplus::Rect ( rcDirty->right - f->padding.right , rcDirty->top , 20 , rcDirty->bottom - rcDirty->top + 1 );
			g.FillRectangle ( brush , ScrollRect );
			delete brush;

			//ползунок
			brush = new Gdiplus::SolidBrush ( background - 38 );
			int c = std::max ( f->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
			Gdiplus::Rect ScrollBar = Gdiplus::Rect ( rcDirty->right - f->padding.right + 2 , rcDirty->top + ( ( double ) f->VPos ) / f->MaxTextHeight * ( f->getAbsoluteSize ().height - c ) , 16 , c );
			g.FillRectangle ( brush , ScrollBar );
			delete brush;
		}
		
		RECT r = RECT ( *rcDirty );
		r.top = -f->VPos;
		r.left = -f->HPos;
		f->padding.reRect ( r );
		drawText(hdc, Gdiplus::Rect ( r.left , r.top , r.right - r.left , r.bottom - r.top ));

		{//рамочка
			padding.right += 30;
			padding.left += 5;
			padding.bottom += 30;
			padding.top += 5;
			brush = new Gdiplus::SolidBrush ( background );
			//left
			Gdiplus::Rect border = Gdiplus::Rect ( rcDirty->left, rcDirty->top, padding.left , rcDirty->bottom - rcDirty->top + 1 );
			g.FillRectangle ( brush , border );
			//top
			border = Gdiplus::Rect ( rcDirty->left, rcDirty->top, rcDirty->right - rcDirty->left + 1, padding.top );
			g.FillRectangle ( brush , border );
			//right
			border = Gdiplus::Rect ( rcDirty->right - padding.right, rcDirty->top, padding.right, rcDirty->bottom - rcDirty->top + 1 );
			g.FillRectangle ( brush , border );
			//bottom
			border = Gdiplus::Rect ( rcDirty->left, rcDirty->top - padding.bottom, rcDirty->right - rcDirty->left + 1, padding.bottom );
			g.FillRectangle ( brush , border );
			delete brush;
		}

		EndPaint(hwnd, &pstruct);
	}
	LRESULT CodeEdit::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, ProcessView *pData)//TODO
	{
		PCodeEdit* f = dynamic_cast < PCodeEdit* > ( pData );
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
			int height = AbsSize.height;
			Padding padding = pData->padding;
			padding.right += 20;
			padding.left += 5;
			padding.bottom += 20;
			padding.top += 5;
			padding.reSize ( AbsSize );
			int cX = std::max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
			int cY = std::max ( pData->getAbsoluteSize ().width * AbsSize.width / f->TextWidth , 20 );
			int aX = pData->getAbsoluteSize ().height;
			int aY = pData->getAbsoluteSize ().width;
			int posX = pData->padding.top + ( ( double ) f->VPos ) / f->MaxTextHeight * ( pData->getAbsoluteSize ().height - cX );
			int posY = pData->padding.left + ( ( double ) f->HPos ) / f->MaxTextWidth * ( pData->getAbsoluteSize ().width - cY );
			int xPos = LOWORD ( lParam );
			int yPos = HIWORD ( lParam );
			if ( posX < yPos && yPos < posX + cX && xPos > width-18 && xPos < width-2) {
				f->isDownV = 1;
				SetCapture ( hwnd );
				f->oldY = yPos;
			}
			if ( posY < yPos && yPos < posY + cY && xPos > height-18 && xPos < height-2) {
				f->isDownH = 1;
				SetCapture ( hwnd );
				f->oldX = xPos;
			}
			SetFocus(hwnd);
		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( f->isDownV ) {
				Size AbsSize = pData->getAbsoluteSize ();
				pData->padding.toAbsolut(AbsSize).reSize ( AbsSize );
				int c = std::max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
				int a = pData->getAbsoluteSize ().height;
				short yPos = HIWORD ( lParam );

				int deltaPos = yPos - f->oldY;
				f->VPos += deltaPos * f->MaxTextHeight / ( a - c );
				if ( f->VPos > f->MaxTextHeight )
				{
					f->VPos = f->MaxTextHeight;
				}

				if ( f->VPos < 0 )
				{
					f->VPos = 0;
				}
				f->oldY = yPos;
				InvalidateRect ( hwnd , 0 , 0 );
			}
			if ( f->isDownH ) {
				Size AbsSize = pData->getAbsoluteSize ();
				pData->padding.toAbsolut(AbsSize).reSize ( AbsSize );
				int c = std::max ( pData->getAbsoluteSize ().width * AbsSize.width / f->TextWidth , 20 );
				int a = pData->getAbsoluteSize ().width;
				short xPos = LOWORD ( lParam );

				int deltaPos = xPos - f->oldX;
				f->VPos += deltaPos * f->MaxTextWidth / ( a - c );
				if ( f->VPos > f->MaxTextWidth )
				{
					f->VPos = f->MaxTextWidth;
				}

				if ( f->VPos < 0 )
				{
					f->VPos = 0;
				}
				f->oldX = xPos;
				InvalidateRect ( hwnd , 0 , 0 );
			}

		}
		break;
		case WM_MOUSELEAVE:
		case WM_LBUTTONUP:
		{
			if( f->isDownV || f->isDownH )
				ReleaseCapture ();
			f->isDownV = 0;
			f->isDownH = 0;
		}
		break;

		case WM_MOUSEWHEEL:
		{
			int WPDelta = GET_WHEEL_DELTA_WPARAM ( wParam );
			int key = LOWORD(wParam);
			WPDelta /= 15;
			if(key & MK_SHIFT){
				f->VPos -= WPDelta;
				if ( f->VPos > f->MaxTextHeight )
				{
					f->VPos = f->MaxTextHeight;
				}
				
				if ( f->VPos < 0 )
				{
					f->VPos = 0;
				}
			}
			else{
				f->HPos -= WPDelta;
				if ( f->HPos > f->MaxTextWidth )
				{
					f->HPos = f->MaxTextWidth;
				}
				
				if ( f->HPos < 0 )
				{
					f->HPos = 0;
				}
			}
			InvalidateRect ( hwnd, 0, 0 );
			return 0;
		}
		case WM_SIZE:
		case WM_SETSCROLLEDTEXT:
		{
			Size buff = f->getAbsoluteSize();
			f->padding.reSize ( buff );
			Size rect1 ( (short)buff.width , 0 );
			Size rect2;
			Gdiplus::Graphics Graphs ( GetDC ( 0 ) );
			//Graphs.MeasureString ( text.c_str () , -1 , font , rect1 , stringFormat , &rect2 , 0 , 0 );
			rect_text(model, rect1, rect2);
			f->MaxTextHeight = rect2.height - buff.height + 1;
			f->TextHeight = rect2.height + 1;
			if(f->MaxTextHeight > 0) f->padding.right = 20;
			else f->padding.right = 0;
			if ( f->VPos > f->MaxTextHeight )
			{
				//f->padding.right = 20;
				f->VPos = f->MaxTextHeight;
			}
			if ( f->VPos < 0 )
			{
				//f->padding.right = 0;
				f->VPos = 0;
			}
			f->MaxTextWidth = rect2.width - buff.width + 1;
			f->TextWidth = rect2.width + 1;
			if(f->MaxTextWidth > 0) f->padding.bottom = 20;
			else f->padding.bottom = 0;
			if ( f->HPos > f->MaxTextWidth )
			{
				//f->padding.right = 20;
				f->HPos = f->MaxTextWidth;
			}
			if ( f->HPos < 0 )
			{
				//f->padding.right = 0;
				f->HPos = 0;
			}
			InvalidateRect(hwnd, 0, 1);
			return 0;
		}
		case WM_CHAR:{
			wchar_t ch = wParam;
			if(ch == L'\b')
				return 0;
			text.add(ch, text_cursor);
			insert_to_model(GetDC(0), model, ch, text_cursor);
			text_cursor++;
			SendMessage ( PV->hwnd , WM_SETSCROLLEDTEXT , 0 , 0 );
			return 0;
		}
		case WM_KEYDOWN: {
            // Обрабатываем нажатия стрелочек
            switch (wParam) {
                case VK_RIGHT:
					if(text_cursor != text.size)
						text_cursor++;
					InvalidateRect(hwnd, 0, 1);
                    break;
                case VK_LEFT:
					if(text_cursor != 0)
						text_cursor--;
					InvalidateRect(hwnd, 0, 1);
                    break;
				case VK_BACK:
					if(text_cursor){
						text.del(text_cursor);
						erase_to_model(GetDC(0), model, text_cursor);
					}
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
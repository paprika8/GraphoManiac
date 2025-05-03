#pragma once
#include "../oopWin.h"
#include <functional>
#include "../positioner.h"
namespace BoolApp
{

	class PButton : public ProcessView
	{
	public:
		bool isDown = 0;
		PButton(HWND ahwnd, View *aview);

		Size GetContentSize(Size size) override {
			if(margin.type & MarginType::CONTENT)
				return 
			return size;
		}

		void construction() override;

	private:
	};

	class Button : public View
	{
	public:
		std::wstring text = L"";
		Gdiplus::Color text_color = Gdiplus::Color(0, 0, 0);
		int flag_format = 0;
		std::function<void(Button *)> click = [](Button *) -> void {};
		HFONT font = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
								 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
		bool safe_down = 0;

		Button(Builder *abuilder = new DefaultBuilder()) : View(abuilder)
		{
			resize = right_form;
		}

		void set_font_size(int size)
		{
			DeleteObject(font);
			font = CreateFontA(
				size,
				0,
				0,
				0,
				FW_NORMAL,
				FALSE,
				FALSE,
				FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_ROMAN,
				"Times New Roman");
			if (PV)
				SendMessage(PV->hwnd, WM_SETFONT, font, true);
		}

		std::wstring getSzWindowClass() override
		{
			return L"Button";
		};

		void Register(WNDCLASS &wca) override
		{
			wca.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(225, 60, 60));
		};

		ProcessView *VConstruct(ProcessView *apv) override
		{
			PButton *pb = new PButton(apv->hwnd, this);
			pb->construction();
			return pb;
		};

		virtual void paint(HWND hwnd);

		void Click()
		{
			click(this);
		}

		LRESULT wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam, ProcessView *ptr) override
		{
			PButton *pButton = (PButton *)ptr;
			switch (message)
			{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			{
				pButton->isDown = true;
				background = background - 30;
				ptr->margin.right -= 5;
				ptr->margin.top += 5;
				ptr->margin.left += 5;
				ptr->margin.bottom -= 5;
				Positioning(ptr->parent);
				InvalidateRect(ahwnd, 0, 0);

				SetCapture(ahwnd);
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			case LBN_KILLFOCUS:
			case WM_MOUSEMOVE:
			{
				LPPOINT point = new POINT();
				if (pButton->isDown && GetCursorPos(point) && WindowFromPoint(*point) == ahwnd)
				{
					delete point;
					return DefWindowProc(ahwnd, message, wparam, lparam);
				}
				delete point;
				pButton->isDown = false;
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			case WM_LBUTTONUP:
			{
				bool event = pButton->isDown;
				
				pButton->isDown = false;
				ptr->margin.right += 5;
				ptr->margin.top -= 5;
				ptr->margin.left -= 5;
				ptr->margin.bottom += 5;
				Positioning(ptr->parent);
				background = background + 30;
				InvalidateRect(ahwnd, 0, 0);

				ReleaseCapture();

				
				if (event){
					//safe_down = !safe_down;
					Click();
				}

				return 1;
				//return DefWindowProc(ahwnd, message, wparam, lparam);
			}
			default:
				return DefWindowProc(ahwnd, message, wparam, lparam);
			}
		};

	private:
	};
}
#pragma once
#include "../Text/Text.h"
#include "../custom_wstr.h"

namespace Graphs
{
	class CodeEdit;

	class PCodeEdit : public ProcessView
	{
		friend CodeEdit;

	public:
		int VPos = 0;
		int HPos = 0;
		int TextHeight = 0;
		int MaxTextHeight = 0;
		int TextWidth = 0;
		int MaxTextWidth = 0;

		PCodeEdit(HWND hwnd, View *aModel) : ProcessView(hwnd, aModel)
		{
			padding.right = 20;
			padding.bottom = 20;
		};

		void construction() override;
		~PCodeEdit()
		{
		}

	private:
		int oldY;
		int oldX;
		bool isDownV = 0;
		bool isDownH = 0;
	};

	class CodeEdit : public View
	{
	public:
		Gdiplus::Color text_color = Gdiplus::Color(0, 0, 0);
		int text_cursor = 0;
		HFONT font = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
								 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
		std::function<void(CodeEdit*)> enter = [](CodeEdit*) -> void {};

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
		
		CodeEdit(Builder *abuilder = 0) : View(abuilder)
		{
			background = Gdiplus::Color(231, 237, 216);
		};

		ProcessView *VConstruct(ProcessView *apv) override
		{
			PCodeEdit *pw = new PCodeEdit(apv->hwnd, this);
			pw->construction();
			return pw;
		}

		std::wstring getSzWindowClass() override
		{
			return L"CodeEdit";
		};

		void Register(WNDCLASS &wca) override
		{
			wca.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(193, 193, 100));
		};
		void paint(HWND) override;
		LRESULT wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, ProcessView *pData) override;

		void SetText(std::wstring atext)
		{
			text = atext.c_str();
			model = generate_model(GetDC(0), text);
			if (enabled)
			{
				SendMessage(PV->hwnd, WM_SETSCROLLEDTEXT, 0, 0);
			}
		}
		std::wstring GetText()
		{
			return std::wstring(text.data);
		}

	private:
		custom_wstr text = L"";
		text_model model;
		void drawText(HDC hdc, Gdiplus::Rect rect);
	};
}
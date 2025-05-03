#pragma once
#include "../Text/Text.h"
#include "../custom_wstr.h"

namespace BoolApp
{
	class Edit;

	class PEdit : public ProcessView
	{
		friend Edit;

	public:
		int WPos = 0;
		int TextHeight = 0;
		int MaxTextHeight = 0;

		PEdit(HWND hwnd, View *aModel) : ProcessView(hwnd, aModel)
		{
			padding.right = 20;
		};

		void construction() override;
		~PEdit()
		{
		}

	private:
		int oldY;
		bool isDown = 0;
	};

	class Edit : public View
	{
	public:
		Gdiplus::Color text_color = Gdiplus::Color(0, 0, 0);
		int text_cursor = 0;
		int untext_cursor = 0;
		HFONT font = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
								 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
		std::function<void(Edit*)> enter = [](Edit*) -> void {};

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
		
		Edit(Builder *abuilder = 0) : View(abuilder)
		{
			background = Gdiplus::Color(231, 237, 216);
		};

		ProcessView *VConstruct(ProcessView *apv) override
		{
			PEdit *pw = new PEdit(apv->hwnd, this);
			pw->construction();
			return pw;
		}

		std::wstring getSzWindowClass() override
		{
			return L"Text";
		};

		void Register(WNDCLASS &wca) override
		{
			wca.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(193, 193, 100));
		};
		void paint(HWND) override;
		LRESULT wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, ProcessView *pData) override;

		void SetText(std::wstring atext)
		{
			auto tbuf = std::wstring(text.data).substr(0, untext_cursor);
			text = (tbuf + atext).c_str();
			if (enabled)
			{
				SendMessage(PV->hwnd, WM_SETSCROLLEDTEXT, 0, 0);
			}
		}
		std::wstring GetText()
		{
			return std::wstring(text.data).substr(untext_cursor);
		}
		void SetUnEditText(std::wstring atext)
		{
			text = atext.c_str();
			untext_cursor = atext.size();
			if (enabled)
			{
				SendMessage(PV->hwnd, WM_SETSCROLLEDTEXT, 0, 0);
			}
		}

	private:
		custom_wstr text = L"";
		void drawText(HDC hdc, Gdiplus::Rect rect);
		void rect_text(HDC hdc, Gdiplus::Rect rect, Gdiplus::Rect &res);
	};
}
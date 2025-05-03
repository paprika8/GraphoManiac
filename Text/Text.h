#pragma once
#include "../component/component.h"
#include "../composite/composite.h"
//#include <gdiplus.h>
//using namespace Gdiplus;

//#pragma comment (lib, "Gdiplus.lib")

namespace BoolApp {

	class ScrollText;

	class PScrollText : public ProcessView
	{
		friend ScrollText;
	public:
		int WPos = 0;
		int TextHeight = 0;
		int MaxTextHeight = 0;

		PScrollText ( HWND hwnd, View* aModel) : ProcessView ( hwnd, aModel) {
			padding.right = 20;
		};

		void construction() override;
		~PScrollText(){

		}
	private:
		int oldY;
		bool isDown = 0;
	};

	Gdiplus::Font *createFont ( int size );
	const uint WM_SETSCROLLEDTEXT = 0x0401;

	class ScrollText : public View
	{
	public:
		
		Gdiplus::StringFormat* stringFormat = new Gdiplus::StringFormat ();
		Gdiplus::Font* font = createFont ( 16 );
		Gdiplus::Color background;
		Gdiplus::Color text_color = Gdiplus::Color(0, 0, 0);

		ScrollText ( Builder* abuilder = 0 ) : View ( abuilder ) {
			background = Gdiplus::Color ( 231 , 237 , 216 );
		};

		ProcessView* VConstruct(ProcessView* apv) override {
			PScrollText* pw = new PScrollText(apv->hwnd, this);
			pw->construction();
			return pw;
		}

		std::wstring getSzWindowClass() override {
			return L"Text";
		};

		void Register(WNDCLASS& wca) override {
			wca.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(193, 193, 100));
		};
		void paint (HWND) override;
		LRESULT wndProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;

		void SetText ( std::wstring atext) {
			text = atext;
			if ( enabled ) {
				SendMessage ( PV->hwnd , WM_SETSCROLLEDTEXT , 0 , 0 );
			}
		}
		std::wstring GetText () {
			return text;
		}

		~ScrollText(){
			delete stringFormat;
			delete font;
		}

	private:
		std::wstring text = L"";
	};



}



#pragma once
#include "../component/component.h"
#include "../composite/composite.h"

namespace BoolApp{

	enum ShowType : char{
		HIDE,
		NORMAL,
		MINIMIZED,
		MAXIMIZE,
		NOACTIVATE,
		SHOW,
		MINIMIZE,
		MINNOACTIVE,
		NA,
		RESTORE,
		DEFAULT,
		FORCEMINIMIZE
		//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	};

	class PWindow : public PComponent{
		public:
		PWindow(HWND ahwnd, View* aview) : PComponent(ahwnd, aview){
			
		};

		void construction() override;
		~PWindow(){
			PostQuitMessage ( 0 );
		}
		private:

	};

	class Window : public Component{
		public:
		Window(Builder *abuilder = new DefaultBuilder()) : Component(abuilder){
			Construct();
		}
		std::wstring getSzWindowClass() override {
			return L"Rect";
		};
		
		void Register(WNDCLASS& wca) override {
			wca.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(193, 193, 193));
		};

		ProcessView* VConstruct(ProcessView* apv) override {
			ProcessView* pw = new PWindow(0, this);
			pw->construction();
			return pw;
		};

		void Show(ShowType type){
			if(PV)
				ShowWindow(PV->hwnd, type);
		}

		LRESULT wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam, ProcessView* ptr) override;
	};
}
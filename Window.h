#pragma once

#include <string>
#include <Windows.h>

#include "Views.h"
#include "Screen.h"



namespace Graphs
{

	extern HINSTANCE instance;

	class Window{
	public:
		View* screen = 0;
		HWND hwnd = 0;
		Size_ size = Size_(0,0);
		

		Window(){
			screen = new Screen();
			screen->move(0, 0);
		}

		//получение названия класса окна
		virtual std::wstring getSzWindowClass();

		//регистрирование окна
		virtual void regist();

		HDC getDC(){
			return GetDC(hwnd);
		}

		//отобразить окно
		void show(int type);

		~Window(){
			PostQuitMessage(0);
		}
	private:
		//функция обработки событий окна, можно не трогать, не вчитываться
		static LRESULT wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam);
	};
}
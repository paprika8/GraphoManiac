#pragma once
#include "../component/component.h"
#include "../composite/composite.h"

namespace BoolApp
{

	class PLContainer : public PComposite
	{
	public:
		PLContainer(HWND ahwnd, View *aview) : PComposite(ahwnd, aview) {

		};

		void construction() override;

		virtual Size GetContentSize ( Size size );

		~PLContainer()
		{
			
		}
	};

	class LinearContainer : public Composite
	{
	public:
		LinearContainer(Builder *abuilder = new DefaultBuilder()) : Composite(abuilder)
		{
		}

		std::wstring getSzWindowClass() override {
			return L"LinCont";
		};
		
		void Register(WNDCLASS& wca) override {
			wca.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 244, 204));
		};

		ProcessView* VConstruct(ProcessView* apv) override {
			PLContainer* pw = new PLContainer(apv->hwnd, this);
			PV = pw;
			pw->construction();
			pw->is_vert_orientation = is_vert_orientation;
			ChildsConstruct(apv);
			return pw;
		};

		LRESULT wndProc(HWND ahwnd, UINT message, WPARAM wparam, LPARAM lparam, ProcessView* ptr) override;

	private:
	};
}

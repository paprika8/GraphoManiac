#pragma once
#include "../oopWin.h"
#include "../custom_vector.h"

namespace BoolApp{
	
	class PComponent : public ProcessView{
		public:
		ProcessView* child = 0;

		PComponent(HWND ahwnd, View* aview) : ProcessView(ahwnd, aview){

		}

		~PComponent(){
			delete child;
		}
		virtual Size GetContentSize ( Size size ) {
			Size res;
			Size asize;
			Margin amargin (0, 0, 0, 0);
			asize = getPView ()->size.toAbsolut (size);
			amargin = getPView ()->margin.toAbsolut (size);
			res = res.plusRight (asize, amargin);
			return res;
		}

		void add (ProcessView* apview);

		void remove (ProcessView* apview){
			delete child;
			child = 0;
		}

		ProcessView* getPView (int i = 0){
			return child;
		}



		private:
	};

	class Component : public View{

		public:
		ProcessView* VConstruct(ProcessView* apview) override{
			child->Construct();
			((PComponent*)PV)->add(child->PV);
			
		}
		Component(Builder *abuilder = new DefaultBuilder()) : View(abuilder)
		{

		}

		~Component(){
			delete child;
		}

		void add (View* aview);

		void remove (View*){
			delete child;
			child = 0;
		}

		View* getView (int i){
			return child;
		}

		void enable() override{
			child->enable();
			View::enable();
		}

		void disable() override{
			child->disable();
			View::disable();
		}

		void childDeleted(View* aview) override{
			child = 0;
			if(PV)
				((PComponent*)PV)->child = 0;
		}

		void paint(HWND hwnd) override {
			this->View::paint(hwnd);
			if(child)
				child->paint(child->PV->hwnd);
		};

		private:
		View* child = 0;
	};
}



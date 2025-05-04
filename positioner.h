#pragma once
#include <Windows.h>
#include "oopWin.h"
#include "UIStructs.h"
#include <set>
#include <string>
#include "component/component.h"
#include "composite/composite.h"
namespace Graphs 
{
	class Positionable
	{
	public:
		virtual void position() = 0;
	};
	void PositioningOne(PComponent *comp);
	void PositioningFew(PComposite *compos);
	void Positioning(ProcessView *pv);
}
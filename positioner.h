#pragma once
#include <Windows.h>
#include "Views.h"
#include "UIStructs.h"
#include <set>
#include <string>
namespace Graphs 
{
	class Positionable
	{
	public:
		virtual void position() = 0;
	};

	void Positioning(View *pv, int deep = 0);
}
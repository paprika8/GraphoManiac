#pragma once
#include <Windows.h>
#include "window/window.h"
#include "LinearContainer/LinearContainer.h"
#include "button/button.h"
#include "Text/Text.h"
#include "code_edit/CodeEdit.h"
#include "Edit/Edit.h"

const Gdiplus::Color bg (255, 244, 204);
const Gdiplus::Color confirm (127, 176, 105);
const Gdiplus::Color in (230, 170, 104);
const Gdiplus::Color button (183, 54, 34);
const Gdiplus::Color out (29, 26, 5);
const Gdiplus::Color trans (0, 0, 0, 0);
const Gdiplus::Color wrong (192, 64, 66);

const Gdiplus::Color dark_t = out;
const Gdiplus::Color light_t (255, 255, 255);

using namespace Graphs;

const Padding but_pad = Padding(pointUI(10, percent), 0, 0, 0);

extern int DPI;
extern Window* win;

LinearContainer* create_page_main();


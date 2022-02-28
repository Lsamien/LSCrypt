#ifdef __cplusplus 
extern "C" {
#endif 
#include <LCUI.h>
#include <LCDesign.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/textedit.h>
#include "ui.h"
#ifdef __cplusplus 
}
#endif 
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	LCUI_Widget root, pack;

	LCUI_Init();
	LCDesign_Init();
	LCUIDisplay_SetSize(180, 100);
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile("assets/views/main.xml");
	if (!pack) {
		return -1;
	}
	Logger_SetLevel(LOGGER_LEVEL_ALL);
	Widget_Append(root, pack);
	Widget_Unwrap(pack);
	Widget_SetTitleW(root, L"LS Crypt application.");
	Navigation_Init();
	UI_InitMessageView();
	UI_InitNotificationView();
	Navbar_Init();
	return LCUI_Main();
 }

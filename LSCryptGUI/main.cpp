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
#include "LSCryptinc.h"
#include "RegEdit.h"
#define ID_WINDOW_MAIN			"main-window"


#include "resource.h"
#pragma warning (disable:4311)

/** 在 surface 准备好后，设置与 surface 绑定的窗口的图标 */
static void OnSurfaceReady(LCUI_Event e, void* arg)
{
	HWND hwnd;
	HICON icon;
	HINSTANCE instance;
	LCUI_DisplayEvent dpy_ev = (LCUI_DisplayEvent)arg;
	LCUI_Widget window = LCUIWidget_GetById(ID_WINDOW_MAIN);
	LCUI_Surface surface = LCUIDisplay_GetSurfaceOwner(window);
	if (surface != dpy_ev->surface) {
		return;
	}
	instance = (HINSTANCE)LCUI_GetAppData();
	hwnd = (HWND)Surface_GetHandle(surface);
	icon = LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON_MAIN));
#ifdef _WIN64
	SetClassLongPtr(hwnd, GCLP_HICON, (LONG)icon);
#else
	SetClassLong(hwnd, GCLP_HICON, (LONG)icon);
#endif
}

int main(int argc, char** argv)
{
	if (!LoadDLL())
	{
		return 0;
	}

	char  file[MAX_PATH];
	char* pCmdLine = NULL;
	char* pFile = NULL;

	file[0] = 0;

	//文件路径形式, 如(包括双引号): "C:\\123.UFC"	
	if (argc>1)
	{
		pCmdLine = argv[1];
		pFile = file;
		if (*pCmdLine == '\"') pCmdLine++;
		while (*pCmdLine && *pCmdLine != '\"')
		{
			*pFile++ = *pCmdLine++;
		}
		*pFile = '\0';

		//IsFileExists(file);
		if (file[0]/* && IsFileExists(file)*/)
		{
			//解密文件
			if (LoadDLL())
				fnDllOpenCrypt(AnsiToUnicode(file));
			return FALSE;
		}
	}



	LCUI_Widget root, pack;

	LCUI_Init();
	LCDesign_Init();
	LCUIDisplay_SetSize(180, 100);
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile("assets/views/main.xml");
	if (!pack) {
		return -1;
	}
#ifdef DEBUG
	Logger_SetLevel(LOGGER_LEVEL_ALL);
#endif // DEBUG
	Widget_Append(root, pack);
	Widget_Unwrap(pack);
	Widget_SetTitleW(root, L"LS Crypt application.");
	//Widget_SetId(root, ID_WINDOW_MAIN);

	LCUIDisplay_BindEvent(LCUI_DEVENT_READY, OnSurfaceReady, NULL, NULL, NULL);

	Navigation_Init();
	UI_InitMessageView();
	UI_InitNotificationView();
	Navbar_Init();
	return LCUI_Main();
 }

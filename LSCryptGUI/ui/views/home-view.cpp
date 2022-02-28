#ifdef __cplusplus 
extern "C" {
#endif 
#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCDesign.h>
#include "ui.h"
#ifdef __cplusplus 
}
#endif 
#include "LSCryptinc.h"



#define BTN_CHECK	"checkbox-lym"

static void OnClickCheckBoxlym(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	LCUI_Widget btn;

	btn = LCUIWidget_GetById(BTN_CHECK);
	LCUI_BOOL lcbool = CheckBox_IsChecked(btn);
	if (LoadDLL())
	{
		if (lcbool)
		{
			fnDllRegisterServer();
		}
		else
		{
			fnDllUnregisterServer();

		}
		LCUI_BOOL rebool=0;
		if (fnDLLCheckeRegister())
		{
			rebool=1;
		}
		if (lcbool != rebool)
		{
			LCDesign_OpenErrorMessage(L"ERROR!权限错误！请用管理员身份运行程序！",1500);
			CheckBox_SetChecked(btn, rebool);

		}
		else
		{
			LCDesign_OpenInfoMessage(L"Save success!", 1500);
		}
	
		UnLoadDLL();
	}
}
void HomeView_Init(void)
{
	//LCDesign_InitCheckBox();
	LCUI_Widget dropdown, btn;

	btn = LCUIWidget_GetById(BTN_CHECK);
	if (LoadDLL())
	{
		if (fnDLLCheckeRegister())
		{
			CheckBox_SetChecked(btn, 1);
		}
		else
		{
			CheckBox_SetChecked(btn, 0);
		}
		UnLoadDLL();

	}

	Widget_BindEvent(btn, "click", OnClickCheckBoxlym, NULL, NULL);
}

void HomeView_Free(void)
{

}

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
#include "RegEdit.h"
#include "LSCryptinc.h"

#define BTN_SWITCH_MENU	"btn-switch-menu"
#define BTN_SWITCH_PRO	"btn-switch-pro"

static void OnClickBtnSwitchMenu(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	LCUI_Widget btn;

	btn = LCUIWidget_GetById(BTN_SWITCH_MENU);
	LCUI_BOOL lcbool = Switch_IsChecked(btn);
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
		LCUI_BOOL rebool = 0;
		if (fnDLLCheckeRegister())
		{
			rebool = 1;
		}
		if (lcbool != rebool)
		{
			LCDesign_OpenErrorMessage(L"ERROR!权限错误！请用管理员身份运行程序！", 1500);
			Switch_SetChecked(btn, rebool);

		}
		else
		{
			LCDesign_OpenInfoMessage(L"Save success!", 1500);
		}

		UnLoadDLL();
	}
}

static void OnClickBtnSwitchPro(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	LCUI_Widget btn_switch_pro = LCUIWidget_GetById(BTN_SWITCH_PRO);
	LCUI_BOOL lcbool = Switch_IsChecked(btn_switch_pro);
	if (lcbool)
	{
		if (AssocEx(_T("LSC")))
		{
			LCDesign_OpenInfoMessage(L"关联扩展名.LSC文件成功",1500);
		}
		else
		{
			Switch_SetChecked(btn_switch_pro, !lcbool);

			LCDesign_OpenErrorMessage(L"ERROR!关联失败！权限不足！", 1500);
		}
	}
	else
	{
		if (AssocEx(_T("LSC"), FALSE))
		{
			LCDesign_OpenInfoMessage(L"取消关联扩展名.LSC文件成功", 1500);
		}
		else
		{
			LCDesign_OpenErrorMessage(L"ERROR!取消关联失败！权限不足！", 1500);

			Switch_SetChecked(btn_switch_pro, !lcbool);
		}

	}

}

void HomeView_Init(void)
{
	LCUI_Widget btn_switch_menu = LCUIWidget_GetById(BTN_SWITCH_MENU);
	LCUI_Widget btn_switch_pro = LCUIWidget_GetById(BTN_SWITCH_PRO);
	if (LoadDLL())
	{
		if (fnDLLCheckeRegister())
		{
			Switch_SetChecked(btn_switch_menu, 1);
		}
		else
		{
			Switch_SetChecked(btn_switch_menu, 0);
		}

		UnLoadDLL();
	}
	if (AssocUFCCheck(HKEY_CLASSES_ROOT, NULL))
	{
		Switch_SetChecked(btn_switch_pro, 1);
	}
	else
	{
		Switch_SetChecked(btn_switch_pro, 0);

	}
	Widget_BindEvent(btn_switch_menu, "click", OnClickBtnSwitchMenu, NULL, NULL);
	Widget_BindEvent(btn_switch_pro, "click", OnClickBtnSwitchPro, NULL, NULL);
}

void HomeView_Free(void)
{

}

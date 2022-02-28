#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCDesign.h>
#include "ui.h"



#define BTN_CHECK	"checkbox-lym"

static void OnClickCheckBoxlym(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	LCDesign_OpenInfoMessage(L"Read Data File success!", 1500);

}
void HomeView_Init(void)
{
	//LCDesign_InitCheckBox();
	LCUI_Widget dropdown, btn;

	btn = LCUIWidget_GetById(BTN_CHECK);
	CheckBox_SetChecked(btn, 1);
	Widget_BindEvent(btn, "click", OnClickCheckBoxlym, NULL, NULL);


}

void HomeView_Free(void)
{

}

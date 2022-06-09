#include "Button.h"

int32_t controls::Button::getStatus()
{
	return Button_GetCheck(_hWnd);
}

void controls::Button::setStatus(int32_t buttonStatus)
{
	if (_controlType == type::CHECKBOX) Button_SetCheck(_hWnd, buttonStatus);
}

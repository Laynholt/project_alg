#include "Static.h"

LPCWSTR controls::Static::getText() const
{
	WCHAR buf[100];
	Static_GetText(_hWnd, buf, 100);
	return buf;
}

void controls::Static::setText(LPCWSTR text) const
{
	Static_SetText(_hWnd, text);
}

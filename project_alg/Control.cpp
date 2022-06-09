#include "Control.h"

HWND controls::Control::create(DWORD exStyle, LPCWSTR controlType, LPCWSTR controlText, DWORD controlFlag, int32_t x, int32_t y, int32_t width, int32_t height, HWND parentWnd, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    _hWnd = CreateWindowEx(exStyle, controlType, controlText, controlFlag, x, y, width, height, parentWnd, hMenu, hInstance, lpParam);
    _parentHwnd = parentWnd;
    _hMenu = hMenu;
    _controlRect = { x, y, width, height };
    _controlType = -1;

    return _hWnd;
}

void controls::Control::destroy()
{
    DestroyWindow(_hWnd);
}

RECT controls::Control::getControlRect() const
{
    RECT tempRect = { _controlRect.left, _controlRect.top, _controlRect.right, _controlRect.bottom };
    return tempRect;
}

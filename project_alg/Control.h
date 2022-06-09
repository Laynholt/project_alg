#ifndef _CONTROL_H_
#define _CONTROL_H_

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <Windowsx.h>
#include <cstdint>
#include <string>

#include "WidgetRect.h"

#define CONTROL_BUTTON L"BUTTON"
#define CONTROL_CHECKBOX L"BUTTON"
#define CONTROL_EDIT L"EDIT"
#define CONTROL_STATIC L"STATIC"
#define CONTROL_COMBOBOX L"COMBOBOX"
#define CONTROL_GROUPBOX L"BUTTON"

namespace controls
{
	namespace type
	{
		enum controlType
		{
			BUTTON  = 0,
			CHECKBOX = 1,
			COMBOBOX  = 2,
			GROUPBOX  = 3,
			STATIC = 4,
			EDIT  = 5,
		};
	}

	class Control
	{
	public:
		HWND create(DWORD exStyle, LPCWSTR controlType, LPCWSTR controlText, DWORD controlFlag, int32_t x, int32_t y,
			int32_t width, int32_t height, HWND parentWnd, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
		void destroy();

		HWND getHwnd() const { return _hWnd; }
		HWND getParentHwnd() const { return _parentHwnd; }

		int32_t getX() const { return _controlRect.left; }
		int32_t getY() const { return _controlRect.top; }
		int32_t getWidth() const { return _controlRect.width; }
		int32_t getHeight() const { return _controlRect.height; }
		WidgetRect getWidgetRect() const { return _controlRect; }
		RECT getControlRect() const;

		int16_t getControlType() const { return _controlType; }
		HMENU getHMenu() const { return _hMenu; }

		void setControlType(int16_t controlType) { _controlType = controlType; }
		void setWidgetRect(controls::WidgetRect widgetRect) { _controlRect = widgetRect; }

	protected:
		Control() : _hWnd(nullptr), _parentHwnd(nullptr), _hMenu(nullptr) {}

	protected:
		HWND _hWnd, _parentHwnd;
		WidgetRect _controlRect{};
		HMENU _hMenu;
		int16_t _controlType{};
	};
}

#endif // !_CONTROL_H_
#ifndef _WINAPP_H_
#define _WINAPP_H_

#pragma comment (lib, "comctl32")
#include <Windows.h>
#include <Commdlg.h>
#include <commctrl.h>
#include <Windowsx.h>

#include <string>
#include <cstdint>

#include "Config.h"
#include "Window.h"


#define IDN_MAIN_WND TEXT("MAIN_WINDOW")
#define IDN_VISUALIZATION_WND TEXT("VISUALIZATION_WINDOW")
#define IDN_AXIS_WND TEXT("AXIS_WINDOW")
#define IDN_CONTROL_WND TEXT("CONTROL_WINDOW")

// Кастомные сообщения
namespace messages
{
	enum CustomMessage
	{
		ADDED_NEW_OBJECT = WM_USER + 0x0001,
		SELECTED_ANOTHER_PARAMETER = WM_USER + 0x0002,
		SELECTED_ANOTHER_OBJECT = WM_USER + 0x0003,
		CHANGED_OBJECT_DATA = WM_USER + 0x0004
	};
}


// ID виджетов
namespace widgteId
{
	enum WidgetsId
	{
		BUTTON_ADD = 10,

		CHECKBOX_FILL = 11,
		CHECKBOX_DELETE = 12,

		COMBOBOX_OBJECTNAMES = 13,
		COMBOBOX_ALGORITHM = 14,
		COMBOBOX_MATRIX = 15,
		COMBOBOX_OBJECTS = 16,

		GROUPBOX_COORDS = 17,

		EDIT1 = 18,
		EDIT2 = 19,
		EDIT3 = 20,
		EDIT4 = 21,
		EDIT5 = 22,
		EDIT6 = 23,
		EDIT7 = 24,

		LABEL_DRAW_TIME1 = 25,
		LABEL_DRAW_TIME2 = 26
	};
}


class WinApp
{
public:
	WinApp() = delete;
	WinApp(LPCWSTR windowName, HINSTANCE hInstance, LPWSTR lpCmdLine, int32_t nCmdShow);
	~WinApp();

	int16_t create();
	WPARAM run();

private:
	void registration(HINSTANCE hInstance);

	static LRESULT CALLBACK MainWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK VisualizationWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK AxisWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ControlWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK EditSubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

private:
	WNDCLASSEX _wndclass;
	LPWSTR _lpCmdLine;
	int32_t _nCmdShow;
	HINSTANCE _hInstance;
	MSG _msg;

	std::wstring _windowName;

private:
	HFONT _hFontSmall;
	HFONT _hFontNormal;

private:
	controls::Window mainWindow;
	controls::Window visualWindow;
	controls::Window axisWindow;
	controls::Window controlWindow;
};

#endif
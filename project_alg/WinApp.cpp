#include "WinApp.h"
#include <iostream>

WinApp::WinApp(LPCWSTR windowName, HINSTANCE hInstance, LPWSTR lpCmdLine, int32_t nCmdShow)
{
	_lpCmdLine = lpCmdLine;
	_nCmdShow = nCmdShow;
	_hInstance = hInstance;
	_windowName = windowName;

	// Создаем свой шрифт
	HFONT hFontTmp = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT fontData;
	GetObject(hFontTmp, sizeof(LOGFONT), &fontData);
	fontData.lfHeight = -config::widgets::fontSize::tiny;
	_hFontSmall = CreateFontIndirect(&fontData);
	fontData.lfHeight = -config::widgets::fontSize::normal;
	_hFontNormal = CreateFontIndirect(&fontData);
	DeleteObject(hFontTmp);

	// Регистрируем классы
	registration(hInstance);


	/*FILE* conin = stdin;
	FILE* conout = stdout;
	FILE* conerr = stderr;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&conin, "CONIN$", "r", stdin);
	freopen_s(&conout, "CONOUT$", "w", stdout);
	freopen_s(&conerr, "CONOUT$", "w", stderr);
	SetConsoleTitle(L"appconsole");*/
}

WinApp::~WinApp()
{
}

int16_t WinApp::create()
{
	mainWindow.getHwnd();
	mainWindow.create(0, IDN_MAIN_WND, _windowName.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, config::mainWindowWidth, config::mainWindowHeight, nullptr, nullptr, _hInstance, this);
	return (mainWindow.getHwnd()) ? TRUE : FALSE;
}

WPARAM WinApp::run()
{
	HWND hWndMain = mainWindow.getHwnd();
	ShowWindow(hWndMain, _nCmdShow);
	UpdateWindow(hWndMain);

	BOOL ret;
	while ((ret = GetMessage(&_msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1) break;
		else
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
	}

	return _msg.wParam;
}

void WinApp::registration(HINSTANCE hInstance)
{
	// Создаем класс основного окна
	_wndclass.cbSize = sizeof(WNDCLASSEX);
	_wndclass.style = CS_HREDRAW | CS_VREDRAW;
	_wndclass.lpfnWndProc = static_cast<WNDPROC>(MainWindowEventHander);
	_wndclass.cbClsExtra = 0;
	_wndclass.cbWndExtra = 0;
	_wndclass.hInstance = hInstance;
	_wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //hbrBackground = static_cast<HBRUSH>(backgroundColorBrush);
	_wndclass.lpszMenuName = NULL;
	_wndclass.lpszClassName = IDN_MAIN_WND;
	_wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Регистрируем класс основного окна
	RegisterClassEx(&_wndclass);


	// Создаем класс окна визуализации
	WNDCLASSEX visualWndClass;
	visualWndClass.cbSize = sizeof(WNDCLASSEX);
	visualWndClass.style = CS_HREDRAW | CS_VREDRAW;
	visualWndClass.lpfnWndProc = static_cast<WNDPROC>(VisualizationWindowEventHander);
	visualWndClass.cbClsExtra = 0;
	visualWndClass.cbWndExtra = 0;
	visualWndClass.hInstance = hInstance;
	visualWndClass.hIcon = nullptr;
	visualWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	visualWndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH); //hbrBackground = static_cast<HBRUSH>(backgroundColorBrush);
	visualWndClass.lpszMenuName = nullptr;
	visualWndClass.lpszClassName = IDN_VISUALIZATION_WND;
	visualWndClass.hIconSm = nullptr;

	// Регистрируем класс окна визуализации
	RegisterClassEx(&visualWndClass);


	// Создаем класс окна осей
	WNDCLASSEX axisWndClass;
	axisWndClass.cbSize = sizeof(WNDCLASSEX);
	axisWndClass.style = CS_HREDRAW | CS_VREDRAW;
	axisWndClass.lpfnWndProc = static_cast<WNDPROC>(AxisWindowEventHander);
	axisWndClass.cbClsExtra = 0;
	axisWndClass.cbWndExtra = 0;
	axisWndClass.hInstance = hInstance;
	axisWndClass.hIcon = nullptr;
	axisWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	axisWndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //hbrBackground = static_cast<HBRUSH>(backgroundColorBrush);
	axisWndClass.lpszMenuName = nullptr;
	axisWndClass.lpszClassName = IDN_AXIS_WND;
	axisWndClass.hIconSm = nullptr;

	// Регистрируем класс окна осей
	RegisterClassEx(&axisWndClass);

	// Создаем класс окна управления
	WNDCLASSEX controlWndClass;
	controlWndClass.cbSize = sizeof(WNDCLASSEX);
	controlWndClass.style = CS_HREDRAW | CS_VREDRAW;
	controlWndClass.lpfnWndProc = static_cast<WNDPROC>(ControlWindowEventHander);
	controlWndClass.cbClsExtra = 0;
	controlWndClass.cbWndExtra = 0;
	controlWndClass.hInstance = hInstance;
	controlWndClass.hIcon = nullptr;
	controlWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	controlWndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH); //hbrBackground = static_cast<HBRUSH>(backgroundColorBrush);
	controlWndClass.lpszMenuName = nullptr;
	controlWndClass.lpszClassName = IDN_CONTROL_WND;
	controlWndClass.hIconSm = nullptr;

	// Регистрируем класс окна управления
	RegisterClassEx(&controlWndClass);
}

LRESULT CALLBACK WinApp::MainWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WinApp* pThis;
	LRESULT result = 0;

	if (message == WM_NCCREATE)
	{
		pThis = static_cast<WinApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
			return TRUE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WinApp*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
		bool wasHandled = false;

		if (pThis)
		{
			switch (message)
			{
			case WM_CREATE:
				{
					pThis->visualWindow.create(WS_EX_TOPMOST, IDN_VISUALIZATION_WND, nullptr, WS_CHILD | WS_VISIBLE, 0, 0,
						config::mainWindowWidth - config::controlPanelWidth, config::mainWindowHeight, hWnd, nullptr, pThis->_hInstance, pThis);

					pThis->axisWindow.create(WS_EX_TOPMOST, IDN_AXIS_WND, nullptr, WS_CHILD | WS_VISIBLE, config::mainWindowWidth - config::axisPanelWidth,
						0, config::axisPanelWidth, config::axisPanelHeight, hWnd, nullptr, pThis->_hInstance, pThis);

					pThis->controlWindow.create(WS_EX_TOPMOST, IDN_CONTROL_WND, nullptr, WS_CHILD | WS_VISIBLE,
						config::mainWindowWidth - config::controlPanelWidth, config::axisPanelHeight, config::controlPanelWidth,
						config::controlPanelHeight, hWnd, nullptr, pThis->_hInstance, pThis);

					wasHandled = true;
					result = 0;
					break;
				}
			case WM_SIZE:
				{
					if (wParam == SIZE_MINIMIZED) break;

					RECT mainRect;
					GetClientRect(hWnd, &mainRect);
					int32_t mainWidth = mainRect.right - mainRect.left;
					int32_t mainHeight = mainRect.bottom - mainRect.top;

					int32_t visualWidth = mainWidth - config::axisPanelWidth;
					int32_t controlHeight = mainHeight - config::axisPanelHeight;

					pThis->mainWindow.setWidgetRect(controls::WidgetRect(mainRect.left, mainRect.top, mainRect.right - mainRect.left, mainRect.bottom - mainRect.top));

					SetWindowPos(pThis->visualWindow.getHwnd(), nullptr, 0, 0, visualWidth, mainHeight, SWP_NOZORDER);
					SetWindowPos(pThis->axisWindow.getHwnd(), nullptr, visualWidth, 0, config::axisPanelWidth, config::axisPanelHeight, SWP_NOZORDER);
					SetWindowPos(pThis->controlWindow.getHwnd(), nullptr, visualWidth, config::axisPanelHeight, config::controlPanelWidth, controlHeight, SWP_NOZORDER);
					RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW);

					wasHandled = true;
					result = 0;
					break;
				}
			case WM_GETMINMAXINFO:
				{
					LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
					lpMMI->ptMinTrackSize.x = config::mainWindowWidth;
					lpMMI->ptMinTrackSize.y = config::mainWindowHeight;

					wasHandled = true;
					result = 0;
					break;
				}
			case WM_DESTROY:
				{
					PostQuitMessage(0);
					wasHandled = true;
					result = 1;
					break;
				}
			}
		}
		if (!wasHandled)
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return result;
}

LRESULT CALLBACK WinApp::VisualizationWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WinApp* pThis;
	LRESULT result = 0;

	static POINT oldMousePosition{};
	static bool isPressedWithShift = false;

	if (message == WM_NCCREATE)
	{
		pThis = static_cast<WinApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
			return TRUE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WinApp*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
		bool wasHandled = false;

		if (pThis)
		{
			switch (message)
			{
			case WM_CREATE:
				{
					pThis->visualWindow.addDrawingArea(pThis->visualWindow.getWidgetRect().compress(10), config::widgets::color::areaBackground);

					result = 0;
					wasHandled = true;
					break;
				}
			case WM_SIZE:
				{
					RECT visualRect;
					GetClientRect(hWnd, &visualRect);

					// Получаем страрый размер окна
					controls::WidgetRect oldWindowSize = pThis->visualWindow.getWidgetRect();
					controls::WidgetRect newWindowSize = { visualRect.left, visualRect.top,
						visualRect.right - visualRect.left, visualRect.bottom - visualRect.top };

					// Сохраняем новый размер окна
					pThis->visualWindow.setWidgetRect(newWindowSize);

					// Изменяем размер полотна
					pThis->visualWindow.updateAllDrawingAreasSize(newWindowSize.compress(10));

					
					wasHandled = true;
					result = 0;
					break;
				}
			case WM_PAINT:
				{
					// Рисуем поля отрисовки
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd, &ps);

					// Получаем инфу, какая матрица выбрана, какой алгоритм удаления и закраска
					int16_t matrixNumber = reinterpret_cast<controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_MATRIX))->getRow();
					bool isEnableDeleting = reinterpret_cast<controls::Button*>(pThis->controlWindow.getWidget(widgteId::CHECKBOX_DELETE))->getStatus();
					int16_t algorithmNumber = graphics::algorithmType::NONE;
					bool isFillingObjects = reinterpret_cast<controls::Button*>(pThis->controlWindow.getWidget(widgteId::CHECKBOX_FILL))->getStatus();

					if (isEnableDeleting)
						algorithmNumber = reinterpret_cast<controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_ALGORITHM))->getRow();		

					auto drawedTime = pThis->visualWindow.drawAllDrawingAreas(hdc, algorithmNumber, matrixNumber, isFillingObjects);

					EndPaint(hWnd, &ps);

					std::wstring text = std::to_wstring(drawedTime) + L" ms";
					reinterpret_cast<controls::Static*>(pThis->controlWindow.getWidget(widgteId::LABEL_DRAW_TIME2))->setText(text.c_str());

					wasHandled = true;
					result = 0;
					break;
				}
			case WM_KEYDOWN:
				{
					int16_t comboboxRow = reinterpret_cast<::controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_OBJECTS))->getRow();

					if (comboboxRow != -1)
					{
						graphics::Object<float>* objectPtr = pThis->visualWindow.getDrawingAreaByIndex(0)->getObjectByIndex(comboboxRow);
						int16_t editsSize = pThis->controlWindow.getWidgetsTypeSize(controls::type::EDIT);
						float* transforms = new float[editsSize];
						float shift = 2.0f;

						objectPtr->getUserTransformValues(transforms);

						switch (wParam)
						{
						case VK_UP:
						{
							objectPtr->setUserTranslateXY(transforms[0], transforms[1] - shift);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT2))->setText(std::to_wstring(static_cast<int16_t>(transforms[1] - shift)).c_str());
							break;
						}
						case VK_DOWN:
						{
							objectPtr->setUserTranslateXY(transforms[0], transforms[1] + shift);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT2))->setText(std::to_wstring(static_cast<int16_t>(transforms[1] + shift)).c_str());
							break;
						}
						case VK_LEFT:
						{
							objectPtr->setUserTranslateXY(transforms[0] - shift, transforms[1]);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT1))->setText(std::to_wstring(static_cast<int16_t>(transforms[0] - shift)).c_str());
							break;
						}
						case VK_RIGHT:
						{
							objectPtr->setUserTranslateXY(transforms[0] + shift, transforms[1]);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT1))->setText(std::to_wstring(static_cast<int16_t>(transforms[0] + shift)).c_str());
							break;
						}
						case 'W':
						{
							objectPtr->setUserRotateXYX(transforms[3] + shift, transforms[4], transforms[5]);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT4))->setText(std::to_wstring(static_cast<int16_t>(transforms[3] + shift)).c_str());
							break;
						}
						case 'S':
						{
							objectPtr->setUserRotateXYX(transforms[3] - shift, transforms[4], transforms[5]);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT4))->setText(std::to_wstring(static_cast<int16_t>(transforms[3] - shift)).c_str());
							break;
						}
						case 'A':
						{
							objectPtr->setUserRotateXYX(transforms[3], transforms[4], transforms[5] + shift);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT6))->setText(std::to_wstring(static_cast<int16_t>(transforms[5] + shift)).c_str());
							break;
						}
						case 'D':
						{
							objectPtr->setUserRotateXYX(transforms[3], transforms[4], transforms[5] - shift);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT6))->setText(std::to_wstring(static_cast<int16_t>(transforms[5] - shift)).c_str());
							break;
						}
						case 'Q':
						{
							objectPtr->setUserRotateXYX(transforms[3], transforms[4] + shift, transforms[5]);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT5))->setText(std::to_wstring(static_cast<int16_t>(transforms[4] + shift)).c_str());
							break;
						}
						case 'E':
						{
							objectPtr->setUserRotateXYX(transforms[3], transforms[4] - shift, transforms[5]);
							pThis->visualWindow.updateAllDrawingAreas();
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT5))->setText(std::to_wstring(static_cast<int16_t>(transforms[4] - shift)).c_str());
							break;
						}
						default:
							break;
						}
						delete[] transforms;
					}

					wasHandled = true;
					result = 0;
					break;
				}
			case WM_MOUSEMOVE:
			{
				if (isPressedWithShift)
				{
					if ((wParam & MK_LBUTTON) || (wParam & MK_MBUTTON))
					{
						POINT currentMousePosition;
						currentMousePosition.x = GET_X_LPARAM(lParam);
						currentMousePosition.y = GET_Y_LPARAM(lParam);

						int16_t deltaX, deltaY;
						deltaX = currentMousePosition.x - oldMousePosition.x;
						deltaY = currentMousePosition.y - oldMousePosition.y;

						oldMousePosition.x = currentMousePosition.x;
						oldMousePosition.y = currentMousePosition.y;

						int16_t comboboxRow = reinterpret_cast<::controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_OBJECTS))->getRow();

						if (comboboxRow != -1)
						{
							graphics::Object<float>* objectPtr = pThis->visualWindow.getDrawingAreaByIndex(0)->getObjectByIndex(comboboxRow);
							float transforms[7]{};

							objectPtr->getUserTransformValues(transforms);

							deltaX += static_cast<int16_t>(transforms[0]);
							deltaY += static_cast<int16_t>(transforms[1]);

							objectPtr->setUserTranslateXY(deltaX, deltaY);
							pThis->visualWindow.updateAllDrawingAreas();

							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT1))->setText(std::to_wstring(deltaX).c_str());
							reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(widgteId::EDIT2))->setText(std::to_wstring(deltaY).c_str());
						}
					}
				}

				wasHandled = true;
				result = 0;
				break;
			}
			case WM_LBUTTONUP:
				{
					isPressedWithShift = false;
					wasHandled = true;
					result = 0;
					break;
				}
			case WM_LBUTTONDOWN:
				{
				// Ловим ЛКМ с Шифтом и запоминаем точку нажатия
					if (wParam & MK_SHIFT)
					{
						oldMousePosition.x = GET_X_LPARAM(lParam);
						oldMousePosition.y = GET_Y_LPARAM(lParam);
						if (wParam & MK_SHIFT) isPressedWithShift = true;
					}
					SetFocus(hWnd);

					wasHandled = true;
					result = 0;
					break;
				}

			// Кастомные сообщения
			case messages::ADDED_NEW_OBJECT:
			{
				// Количество объектов
				static int16_t objectCount = 0;

				// Считывание объекта, который был выбран
					// SendMessage(hWnd, Msg, numberString, pThis);
				int16_t numberOfString = static_cast<int16_t>(wParam);
				switch (numberOfString)
				{
				case graphics::type::CUBE:
					{
						++objectCount;
						pThis->visualWindow.getDrawingAreaByIndex(0)->addObject(graphics::type::CUBE,
							L"Объект " + std::to_wstring(objectCount) + L" - Куб", config::objectPreset::color::standartColorBlue);
						pThis->visualWindow.updateAllDrawingAreas();
						break;
					}
				case graphics::type::PYRAMID:
					{
						++objectCount;
						pThis->visualWindow.getDrawingAreaByIndex(0)->addObject(graphics::type::PYRAMID,
							L"Объект " + std::to_wstring(objectCount) + L" - Пирамида", config::objectPreset::color::standartColorRed);
						pThis->visualWindow.updateAllDrawingAreas();
						break;
					}
				case graphics::type::PRISM:
					{
						++objectCount;
						pThis->visualWindow.getDrawingAreaByIndex(0)->addObject(graphics::type::PRISM,
							L"Объект " + std::to_wstring(objectCount) + L" - Призма", config::objectPreset::color::standartColorGreen);
						pThis->visualWindow.updateAllDrawingAreas();
						break;
					}
				default:
					MessageBox(NULL, L"Неизвестный объект", L"Ошибка", MB_OK);
					break;
				}

				wasHandled = true;
				result = 0;
				break;
			}
			case messages::SELECTED_ANOTHER_PARAMETER:
			{
				pThis->visualWindow.updateAllDrawingAreas();

				wasHandled = true;
				result = 0;
				break;
			}
			case messages::CHANGED_OBJECT_DATA:
			{
				// Этот код берет данные только из одного бокса
				int16_t editControlsId = static_cast<int16_t>(wParam);
				int16_t objectNumber = reinterpret_cast<controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_OBJECTS))->getRow();
				
				if (objectNumber > -1)
				{
					int16_t value = reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(editControlsId))->getDigit();
					pThis->visualWindow.getDrawingAreaByIndex(0)->getObjectByIndex(objectNumber)->setUserTransformValue(value, editControlsId - widgteId::EDIT1);
					pThis->visualWindow.updateAllDrawingAreas();
				}

				wasHandled = true;
				result = 0;
				break;
			}
			case WM_DESTROY:
				{
					PostQuitMessage(0);
					result = 1;
					wasHandled = true;
					break;
				}
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return result;
}

LRESULT CALLBACK WinApp::AxisWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WinApp* pThis;
	LRESULT result = 0;

	if (message == WM_NCCREATE)
	{
		pThis = static_cast<WinApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
			return TRUE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WinApp*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
		bool wasHandled = false;

		if (pThis)
		{
			switch (message)
			{
			case WM_SIZE:
				{
					RECT axisRect;
					GetClientRect(hWnd, &axisRect);

					// Получаем страрый размер окна
					controls::WidgetRect oldWindowSize = pThis->axisWindow.getWidgetRect();
					controls::WidgetRect newWindowSize = { axisRect.left, axisRect.top,
						axisRect.right - axisRect.left, axisRect.bottom - axisRect.top };

					// Сохраняем новый размер окна
					pThis->axisWindow.setWidgetRect(newWindowSize);


					wasHandled = true;
					result = 0;
					break;
				}
			case WM_DESTROY:
				{
					PostQuitMessage(0);
					result = 1;
					wasHandled = true;
					break;
				}
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return result;
}

LRESULT CALLBACK WinApp::ControlWindowEventHander(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WinApp* pThis;
	LRESULT result = 0;

	static int16_t startEditControlsIndex = 0;

	if (message == WM_NCCREATE)
	{
		pThis = static_cast<WinApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
			return TRUE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WinApp*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
		bool wasHandled = false;

		if (pThis)
		{
			switch (message)
			{
			case WM_CREATE:
				{
					int16_t x, y, width, height, deltaW, deltaH;
					x = config::widgets::standartPaddingWidth;
					y = config::widgets::standartPaddingHeight;

					width = config::controlPanelWidth;
					height = config::controlPanelHeight;
					deltaW = width - 2 * x;
					deltaH = height - 2 * y;

					int16_t controlHeight = config::widgets::standartControlHeight;

					// Checkboxes
					pThis->controlWindow.addControl(controls::type::CHECKBOX, CONTROL_CHECKBOX, L"Закраска", DEFAULT_CHECKBOX, x, y + 4 * (controlHeight + y),
						deltaW / 2, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::CHECKBOX_FILL));
					pThis->controlWindow.addControl(controls::type::CHECKBOX, CONTROL_CHECKBOX, L"Удаление", DEFAULT_CHECKBOX, x + deltaW / 2, y + 4 * (controlHeight + y),
						deltaW / 2, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::CHECKBOX_DELETE));

					// Изначально закраска неактивна, так как стоит алгоритм Варнока
					Button_Enable(pThis->controlWindow.getWidget(widgteId::CHECKBOX_FILL)->getHwnd(), FALSE);

					// Изначально чекбоксы включены
					Button_SetCheck(pThis->controlWindow.getWidget(widgteId::CHECKBOX_FILL)->getHwnd(), TRUE);
					Button_SetCheck(pThis->controlWindow.getWidget(widgteId::CHECKBOX_DELETE)->getHwnd(), TRUE);

					// Comboboxes
					LPCWSTR algorithms[4] = { L"Варнока", L"Робертса", L"Художника", L"Z-буффер" };
					LPCWSTR objectsName[4] = { L"Куб", L"Пирамида", L"Призма" };
					LPCWSTR matrix[3] = { L"Перспективная", L"Изометрическая", L"Кабинетная" };

					pThis->controlWindow.addControl(controls::type::COMBOBOX, CONTROL_COMBOBOX, nullptr, DEFAULT_COMBOBOX, x, y, deltaW, controlHeight * 4, hWnd, reinterpret_cast<HMENU>(widgteId::COMBOBOX_OBJECTNAMES), objectsName, 4);
					pThis->controlWindow.addControl(controls::type::COMBOBOX, CONTROL_COMBOBOX, nullptr, DEFAULT_COMBOBOX, x, y + 2 * (controlHeight + y), deltaW, controlHeight * 4, hWnd, reinterpret_cast<HMENU>(widgteId::COMBOBOX_ALGORITHM), algorithms, 4);
					pThis->controlWindow.addControl(controls::type::COMBOBOX, CONTROL_COMBOBOX, nullptr, DEFAULT_COMBOBOX, x, y + 3 * (controlHeight + y), deltaW, controlHeight * 3, hWnd, reinterpret_cast<HMENU>(widgteId::COMBOBOX_MATRIX), matrix, 3);
					pThis->controlWindow.addControl(controls::type::COMBOBOX, CONTROL_COMBOBOX, nullptr, DEFAULT_COMBOBOX, x, y + 5 * (controlHeight + y), deltaW, controlHeight * 2, hWnd, reinterpret_cast<HMENU>(widgteId::COMBOBOX_OBJECTS), nullptr, NULL);


					// Buttons
					pThis->controlWindow.addControl(controls::type::BUTTON, CONTROL_BUTTON, L"Добавить объект", DEFAULT_BUTTON, x, y +  1 * (controlHeight + y), deltaW, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::BUTTON_ADD));

					// Groupbox
					pThis->controlWindow.addControl(controls::type::GROUPBOX, CONTROL_GROUPBOX, L"Параметры объектов", DEFAULT_GROUPBOX, x, y + 6 * (controlHeight + y), deltaW, 170, hWnd, reinterpret_cast<HMENU>(widgteId::GROUPBOX_COORDS));

					RECT groupboxRect = pThis->controlWindow.getWidget(widgteId::GROUPBOX_COORDS)->getControlRect();
					int16_t widthGb, heightGb;

					widthGb = groupboxRect.right - groupboxRect.left;
					heightGb = groupboxRect.bottom - groupboxRect.top;
					
					// Labels
					int16_t labelHeight = config::widgets::staticControl::standartStaticControlHeight;
					int16_t editWidth = config::widgets::editControl::standartEditControlHeight;

					// TIme label
					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"Время отрисовки: ", DEFAULT_STATIC, x,
						groupboxRect.bottom + y, deltaW / 2, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::LABEL_DRAW_TIME1));
					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"0 ms", DEFAULT_STATIC, x + deltaW / 2,
						groupboxRect.bottom + y, deltaW / 2, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::LABEL_DRAW_TIME2));

					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"X:", DEFAULT_STATIC, groupboxRect.left + 5 + deltaW / 3 + 10 + 5,
						groupboxRect.top + 1 * labelHeight + y, editWidth, controlHeight, hWnd, nullptr);
					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"Y:", DEFAULT_STATIC, groupboxRect.left + deltaW / 3 + editWidth + 25,
						groupboxRect.top + 1 * labelHeight + y, editWidth, controlHeight, hWnd, nullptr);
					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"Z:", DEFAULT_STATIC, groupboxRect.left + deltaW / 3 + 2 * editWidth + 30,
						groupboxRect.top + 1 * labelHeight + y, editWidth, controlHeight, hWnd, nullptr);


					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"Положение:", DEFAULT_STATIC, groupboxRect.left + x, groupboxRect.top + 2 * labelHeight + y,
						deltaW / 3 + 10, controlHeight, hWnd, nullptr);
					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"Поворот:", DEFAULT_STATIC, groupboxRect.left + x, groupboxRect.top + 3 * labelHeight + y,
						deltaW / 3 + 10, controlHeight, hWnd, nullptr);
					pThis->controlWindow.addControl(controls::type::STATIC, CONTROL_STATIC, L"Масштаб:", DEFAULT_STATIC, groupboxRect.left + x, groupboxRect.top + 4 * labelHeight + y,
						deltaW / 3 + 10, controlHeight, hWnd, nullptr);
					
					// Edit
					startEditControlsIndex = pThis->controlWindow.getNumberOfWidgets();
					int16_t endEditControlsIndex = startEditControlsIndex;
					int16_t shift = 0;

					std::vector<HWND> tempEditHwnd;

					tempEditHwnd.push_back(pThis->controlWindow.addControl(controls::type::EDIT, CONTROL_EDIT, nullptr, DEFAULT_EDIT, groupboxRect.left + 5 + deltaW / 3 + 10 + 5,
						groupboxRect.top + 2 * labelHeight + y, editWidth, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::EDIT1)));
					tempEditHwnd.push_back(pThis->controlWindow.addControl(controls::type::EDIT, CONTROL_EDIT, nullptr, DEFAULT_EDIT, groupboxRect.left + deltaW / 3 + editWidth + 25,
						groupboxRect.top + 2 * labelHeight + y, editWidth, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::EDIT2)));
					tempEditHwnd.push_back(pThis->controlWindow.addControl(controls::type::EDIT, CONTROL_EDIT, nullptr, DEFAULT_EDIT, groupboxRect.left + deltaW / 3 + 2 * editWidth + 30,
						groupboxRect.top + 2 * labelHeight + y, editWidth, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::EDIT3)));

					endEditControlsIndex = startEditControlsIndex + tempEditHwnd.size();
					for (int16_t i = startEditControlsIndex + shift; i < endEditControlsIndex; ++i)
					{
						// Устанавливаем численные пределы
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setLimitMax(config::widgets::editControl::positionLimitMax);
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setLimitMin(config::widgets::editControl::positionLimitMin);
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setTextLimit(config::widgets::editControl::digitLimitSize);
					}

					tempEditHwnd.push_back(pThis->controlWindow.addControl(controls::type::EDIT, CONTROL_EDIT, nullptr, DEFAULT_EDIT, groupboxRect.left + 5 + deltaW / 3 + 10 + 5,
						groupboxRect.top + 3 * labelHeight + y, editWidth, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::EDIT4)));
					tempEditHwnd.push_back(pThis->controlWindow.addControl(controls::type::EDIT, CONTROL_EDIT, nullptr, DEFAULT_EDIT, groupboxRect.left + deltaW / 3 + editWidth + 25,
						groupboxRect.top + 3 * labelHeight + y, editWidth, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::EDIT5)));
					tempEditHwnd.push_back(pThis->controlWindow.addControl(controls::type::EDIT, CONTROL_EDIT, nullptr, DEFAULT_EDIT, groupboxRect.left + deltaW / 3 + 2 * editWidth + 30,
						groupboxRect.top + 3 * labelHeight + y, editWidth, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::EDIT6)));

					shift = endEditControlsIndex - startEditControlsIndex;
					endEditControlsIndex = startEditControlsIndex + tempEditHwnd.size();
					for (int16_t i = startEditControlsIndex + shift; i < endEditControlsIndex; ++i)
					{
						// Устанавливаем численные пределы
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setLimitMax(config::widgets::editControl::rotateLimitMax);
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setLimitMin(config::widgets::editControl::rotateLimitMin);
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setTextLimit(config::widgets::editControl::digitLimitSize);
					}

					tempEditHwnd.push_back(pThis->controlWindow.addControl(controls::type::EDIT, CONTROL_EDIT, nullptr, DEFAULT_EDIT, groupboxRect.left + 5 + deltaW / 3 + 10 + 5,
						groupboxRect.top + 4 * labelHeight + y, 3 * editWidth + 2 * x, controlHeight, hWnd, reinterpret_cast<HMENU>(widgteId::EDIT7)));

					shift = endEditControlsIndex - startEditControlsIndex;
					endEditControlsIndex = startEditControlsIndex + tempEditHwnd.size();
					for (int16_t i = startEditControlsIndex + shift; i < endEditControlsIndex; ++i)
					{
						// Устанавливаем численные пределы
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setLimitMax(config::widgets::editControl::scaleLimitMax);
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setLimitMin(config::widgets::editControl::scaleLimitMin);
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setTextLimit(config::widgets::editControl::digitLimitSize);
					}

					for (auto& edit : tempEditHwnd)
					{
						SetWindowSubclass(edit, EditSubClassProc, 0, reinterpret_cast<DWORD_PTR>(pThis));
						// Устанавливаем начальное значения
						SendMessageW(edit, EN_CHANGE, NULL, reinterpret_cast<DWORD_PTR>(pThis));
					}
					tempEditHwnd.clear();

					pThis->controlWindow.setFontAllWidgets(pThis->_hFontNormal);

					result = 0;
					wasHandled = true;
					break;
				}
				// Обрабочик дочерних окон
			case WM_COMMAND:
				{
					// Активируем / Де- кнопку закраски
					int32_t isAlgorithmEnable = reinterpret_cast<controls::Button*>(pThis->controlWindow.getWidget(widgteId::CHECKBOX_DELETE))->getStatus();
					
					if (!isAlgorithmEnable)
						Button_Enable(pThis->controlWindow.getWidget(widgteId::CHECKBOX_FILL)->getHwnd(), TRUE);
					else
					{

						controls::Combobox* algorithms = reinterpret_cast<controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_ALGORITHM));
						int32_t currentRow = algorithms->getRow();

						if (currentRow == 0 || currentRow == 3)
							Button_Enable(pThis->controlWindow.getWidget(widgteId::CHECKBOX_FILL)->getHwnd(), FALSE);
						else
							Button_Enable(pThis->controlWindow.getWidget(widgteId::CHECKBOX_FILL)->getHwnd(), TRUE);
					}

					// Обработка кнопки добавить
					if (LOWORD(wParam) == widgteId::BUTTON_ADD)
					{
						int16_t row = reinterpret_cast<controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_OBJECTNAMES))->getRow();
						SendMessageW(pThis->visualWindow.getHwnd(), messages::ADDED_NEW_OBJECT, row, reinterpret_cast<LPARAM>(pThis));
						SendMessageW(pThis->controlWindow.getHwnd(), messages::ADDED_NEW_OBJECT, NULL, reinterpret_cast<LPARAM>(pThis));
					}
					// Пришел сигнал об изменении данных в боксах
					else if (HIWORD(wParam) == EN_CHANGE)
					{
						//SendMessageW(pThis->visualWindow.getHwnd(), messages::CHANGED_OBJECT_DATA, startEditControlsIndex, reinterpret_cast<LPARAM>(pThis));
						SendMessageW(pThis->visualWindow.getHwnd(), messages::CHANGED_OBJECT_DATA, LOWORD(wParam), reinterpret_cast<LPARAM>(pThis));
					}
					// Если изменили текущий объект
					else if (LOWORD(wParam) == widgteId::COMBOBOX_OBJECTS)
					{
						SendMessageW(pThis->controlWindow.getHwnd(), messages::SELECTED_ANOTHER_OBJECT, NULL, reinterpret_cast<LPARAM>(pThis));
					}
					// Если изменили параметры отрисовки
					else if (LOWORD(wParam) == widgteId::COMBOBOX_ALGORITHM || LOWORD(wParam) == widgteId::COMBOBOX_MATRIX
						|| LOWORD(wParam) == widgteId::CHECKBOX_DELETE || LOWORD(wParam) == widgteId::CHECKBOX_FILL)
					{
						SendMessageW(pThis->visualWindow.getHwnd(), messages::SELECTED_ANOTHER_PARAMETER, NULL, reinterpret_cast<LPARAM>(pThis));
					}

					result = 0;
					wasHandled = true;
					break;
				}
			case WM_SIZE:
				{
					RECT controlRect;
					GetClientRect(hWnd, &controlRect);

					// Получаем страрый размер окна
					controls::WidgetRect oldWindowSize = pThis->controlWindow.getWidgetRect();
					controls::WidgetRect newWindowSize = { controlRect.left, controlRect.top,
						controlRect.right - controlRect.left, controlRect.bottom - controlRect.top };

					// Сохраняем новый размер окна
					pThis->controlWindow.setWidgetRect(newWindowSize);


					wasHandled = true;
					result = 0;
					break;
				}
			// Камстомные сообщения
			case messages::ADDED_NEW_OBJECT:
				{
					controls::Combobox* comboboxPtr = reinterpret_cast<controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_OBJECTS));
					int16_t comboboxSize = comboboxPtr->getSize();

					graphics::Object<float>* objectPtr = pThis->visualWindow.getDrawingAreaByIndex(0)->getObjectByIndex(comboboxSize);
					comboboxPtr->addRow(objectPtr->name.c_str());
					comboboxPtr->setRow(comboboxSize);

					int16_t editsSize = pThis->controlWindow.getWidgetsTypeSize(controls::type::EDIT);

					float* transforms = new float[editsSize];
					objectPtr->getUserTransformValues(transforms);

					// Задаем начальные значения
					for (int16_t i = startEditControlsIndex, j = 0; i < startEditControlsIndex + editsSize; ++i, ++j)
					{
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setText(
							std::to_wstring(static_cast<int16_t>(transforms[j])).c_str());
					}
					delete[] transforms;

					wasHandled = true;
					result = 0;
					break;
				}
			case messages::SELECTED_ANOTHER_OBJECT:
				{
					int16_t comboboxCurrentRow = reinterpret_cast<controls::Combobox*>(pThis->controlWindow.getWidget(widgteId::COMBOBOX_OBJECTS))->getRow();
					graphics::Object<float>* objectPtr = pThis->visualWindow.getDrawingAreaByIndex(0)->getObjectByIndex(comboboxCurrentRow);

					int16_t editsSize = pThis->controlWindow.getWidgetsTypeSize(controls::type::EDIT);

					float* transforms = new float[editsSize];
					objectPtr->getUserTransformValues(transforms);

					// Устанавливаем их в боксы
					for (int16_t i = startEditControlsIndex, j = 0; i < startEditControlsIndex + editsSize; ++i, ++j)
					{
						reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidgetByIndex(i))->setText(
							std::to_wstring(static_cast<int16_t>(transforms[j])).c_str());
					}
					delete[] transforms;

					wasHandled = true;
					result = 0;
					break;
				}
			case WM_DESTROY:
				{
					PostQuitMessage(0);
					result = 1;
					wasHandled = true;
					break;
				}
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return result;
}

LRESULT CALLBACK WinApp::EditSubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	WinApp* pThis = reinterpret_cast<WinApp*>(dwRefData);

	switch (message)
	{
	case WM_CHAR:
		{
			WORD type;
			DWORD start, end;
			WCHAR ch = static_cast<WCHAR>(wParam);

			SendMessage(hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&start), reinterpret_cast<LPARAM>(&end));

			if (ch < L' ') break;																// let control character through
			else if (start == 0 && ch == L'-') break;											// let hyphen-minus through
			else if (start == 0 && ch == L'\x2212') break;										// let Unicode minus sign through
			else if (GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_DIGIT)) break;		// let digit through

			MessageBeep(0);												// otherwise invalid
			break;
		}
	case EN_CHANGE:
	{
		// Устанавливаем 0 по умолчанию в боксы
		SetDlgItemInt(GetParent(hWnd), GetDlgCtrlID(hWnd), 0, TRUE);
		break;
	}
	case WM_MOUSEWHEEL:
		{
			BOOL tmp;
			int32_t idControl = GetDlgCtrlID(hWnd);
			int16_t data = GetDlgItemInt(GetParent(hWnd), idControl, &tmp, TRUE);

			int16_t limitMax = reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(hWnd))->getLimitMax();
			int16_t limitMin = reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(hWnd))->getLimitMin();

			data += ((static_cast<int16_t>(HIWORD(wParam)) > 0) ? 10 : -10);

			if (data > limitMax) data = limitMin;
			else if (data < limitMin) data = limitMax;

			SetDlgItemInt(GetParent(hWnd), idControl, static_cast<UINT>(data), TRUE);
			SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(idControl, EN_CHANGE), 0);

			break;
		}
	case WM_KEYDOWN:
		{
			BOOL tmp;
			auto idControl = GetDlgCtrlID(hWnd);
			int16_t data = GetDlgItemInt(GetParent(hWnd), idControl, &tmp, TRUE);

			int16_t limitMax = reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(hWnd))->getLimitMax();
			int16_t limitMin = reinterpret_cast<controls::Edit*>(pThis->controlWindow.getWidget(hWnd))->getLimitMin();

			switch (wParam) 
			{
			case VK_UP:
				if (data >= limitMax) data = limitMin;
				SetDlgItemInt(GetParent(hWnd), idControl, static_cast<UINT>(data + 1), TRUE);
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(idControl, EN_CHANGE), 0);
				break;

			case VK_DOWN:
				if (data <= limitMin) data = limitMax;
				SetDlgItemInt(GetParent(hWnd), idControl, static_cast<UINT>(data - 1), TRUE);
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(idControl, EN_CHANGE), 0);
				break;
			}
			break;
		}
	case WM_PAINT:
		{
			HDC hdc;
			RECT rect;
			HFONT hFont;
			RECT dimensions;
			TEXTMETRIC textMetric;

			hdc = GetDC(hWnd);
			GetClientRect(hWnd, &dimensions);

			int16_t height = dimensions.bottom - dimensions.top;
			hFont = reinterpret_cast<HFONT>(SendMessage(hWnd, WM_GETFONT, 0, 0));

			SelectFont(hdc, hFont);
			GetTextMetrics(hdc, &textMetric);

			rect = { 0, (height - textMetric.tmHeight) / 2, (dimensions.right - dimensions.left), height - (height - textMetric.tmHeight) / 2 };
			SendMessage(hWnd, EM_SETRECT, 0, reinterpret_cast<LPARAM>(&rect));
			ReleaseDC(hWnd, hdc);

			break;
		}

	case WM_NCDESTROY: 
		{
			RemoveWindowSubclass(hWnd, EditSubClassProc, uIdSubclass);
			break;
		}
	}

	return DefSubclassProc(hWnd, message, wParam, lParam);
}
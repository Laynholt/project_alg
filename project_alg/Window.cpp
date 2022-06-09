#include "Window.h"

controls::Window::~Window()
{
	deleteAllWidgets();
}

HWND controls::Window::addControl(int32_t typeId, LPCWSTR typeName, LPCWSTR text, int32_t flags, int32_t x, 
	int32_t y, int32_t width, int32_t height, HWND parentHwnd, HMENU hMenu, LPCWSTR elements[], int16_t numberElements)
{
	Control* tempWidget;
	widgets.push_back(tempWidget);
	
	HWND hWndWidget{};

	int16_t size = widgets.size();
	switch (typeId)
	{
	case type::BUTTON:
		{
			widgets[size - 1] = new controls::Button(type::BUTTON);
			hWndWidget = widgets[size - 1]->create(0, CONTROL_BUTTON, text, flags, x, y, width, height, parentHwnd, hMenu, NULL, NULL);
		}
		break;
	case type::CHECKBOX:
		{
			widgets[size - 1] = new controls::Button(type::CHECKBOX);
			hWndWidget = widgets[size - 1]->create(0, CONTROL_CHECKBOX, text, flags, x, y, width, height, parentHwnd, hMenu, NULL, NULL);
		}
		break;
	case type::COMBOBOX:
		{
			widgets[size - 1] = new controls::Combobox();
			hWndWidget = widgets[size - 1]->create(0, CONTROL_COMBOBOX, L"", flags, x, y, width, height, parentHwnd, hMenu, NULL, NULL);

			for (int16_t i = 0; i < numberElements; ++i)
			{
				reinterpret_cast<Combobox*>(widgets[size - 1])->addRow(elements[i]);
				//SendMessageW(hWndCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(elements[i]));
			}
			reinterpret_cast<Combobox*>(widgets[size - 1])->setRow(0);
		}
		break;
	case type::GROUPBOX:
		{
			widgets[size - 1] = new controls::Groupbox();
			hWndWidget = widgets[size - 1]->create(0, CONTROL_GROUPBOX, text, flags, x, y, width, height, parentHwnd, hMenu, NULL, NULL);
		}
		break;
	case type::STATIC:
		{
			widgets[size - 1] = new controls::Static();
			hWndWidget = widgets[size - 1]->create(0, CONTROL_STATIC, text, flags, x, y, width, height, parentHwnd, hMenu, NULL, NULL);
		}
		break;
	case type::EDIT:
		{
			widgets[size - 1] = new controls::Edit();
			hWndWidget = widgets[size - 1]->create(0, CONTROL_EDIT, text, flags, x, y, width, height, parentHwnd, hMenu, NULL, NULL);
		}
		break;
	default:
		return nullptr;
		break;
	}
	widgets[size - 1]->setControlType(typeId);

	return hWndWidget;
}

int16_t controls::Window::addDrawingArea(controls::WidgetRect areaSize, uint32_t areaColor)
{
	drawingAreas.push_back(std::move(graphics::DrawingArea(areaSize, config::widgets::color::areaBackground)));
	return drawingAreas[drawingAreas.size() - 1].getAreaId();
}

void controls::Window::deleteDrawingArea(int16_t areaId)
{
	int16_t size = drawingAreas.size();
	for (int16_t i = 0; i < size; ++i)
	{
		if (drawingAreas[i].getAreaId() == areaId)
		{
			//drawingAreas.erase(drawingAreas.begin() + i);
			return;
		}
	}
}

void controls::Window::deleteAllDrawingArea()
{
	drawingAreas.clear();
}

void controls::Window::deleteWidget(HWND hWnd)
{
	int16_t size = widgets.size();

	for (int16_t i = 0; i < size; ++i)
	{
		if (widgets[i]->getHwnd() == hWnd)
		{
			widgets[i]->destroy();
			delete widgets[i];
			widgets.erase(widgets.begin() + i);
			return;
		}
	}
}

void controls::Window::deleteAllWidgets()
{
	int16_t size = widgets.size();
	if (size == 0) return;

	for (int16_t i = 0; i < size; ++i)
	{
		widgets[i]->destroy();
		delete widgets[i];
	}
	widgets.clear();
}

int16_t controls::Window::getWidgetType(HWND hWnd) const
{
	int16_t size = widgets.size();

	for (int16_t i = 0; i < size; ++i)
	{
		if (widgets[i]->getHwnd() == hWnd)
			return widgets[i]->getControlType();
	}
	return -1;
}

graphics::DrawingArea* controls::Window::getDrawingArea(int16_t areaId)
{
	int16_t size = drawingAreas.size();
	for (int16_t i = 0; i < size; ++i)
	{
		if (drawingAreas[i].getAreaId() == areaId)
			return &drawingAreas[i];
	}
	return nullptr;
}

graphics::DrawingArea* controls::Window::getDrawingAreaByIndex(int16_t index)
{
	if (index >= drawingAreas.size() || index < 0) return nullptr;
	return &drawingAreas[index];
}

controls::Control* controls::Window::getWidget(int32_t widgetId)
{
	int16_t size = widgets.size();

	for (int16_t i = 0; i < size; ++i)
	{
		if (widgets[i]->getHMenu() == reinterpret_cast<HMENU>(widgetId))
			return widgets[i];
	}
	return nullptr;
}

controls::Control* controls::Window::getWidget(HWND hWnd)
{
	int16_t size = widgets.size();

	for (int16_t i = 0; i < size; ++i)
	{
		if (widgets[i]->getHwnd() == hWnd)
			return widgets[i];
	}
	return nullptr;
}

controls::Control* controls::Window::getWidgetByIndex(int16_t index)
{
	if (index >= widgets.size() || index < 0) return nullptr;
	return widgets[index];
}

int16_t controls::Window::getWidgetsTypeSize(int16_t widgetType)
{
	int16_t widgetsCurrentTypeSize{};
	int16_t size = widgets.size();

	for (int16_t i = 0; i < size; ++i)
	{
		if (widgets[i]->getControlType() == widgetType)
			++widgetsCurrentTypeSize;
	}
	return widgetsCurrentTypeSize;
}

int16_t controls::Window::getNumberOfWidgets() const
{
	return widgets.size();
}

void controls::Window::setFontCurrentWidget(int32_t widgetId, HFONT hFont)
{
	int16_t size = widgets.size();

	for (int16_t i = 0; i < size; ++i)
	{
		if (widgets[i]->getHMenu() == reinterpret_cast<HMENU>(widgetId))
			SendMessage(widgets[i]->getHwnd(), WM_SETFONT, (WPARAM)hFont, MAKELONG(TRUE, 0));
	}
}

void controls::Window::setFontAllWidgets(HFONT hFont)
{
	int16_t size = widgets.size();

	for (int16_t i = 0; i < size; ++i)
	{
		SendMessage(widgets[i]->getHwnd(), WM_SETFONT, (WPARAM)hFont, MAKELONG(TRUE, 0));
	}
}

void controls::Window::updateDrawingArea(int16_t areaId)
{
	int16_t size = drawingAreas.size();
	for (int16_t i = 0; i < size; ++i)
	{
		if (drawingAreas[i].getAreaId() == areaId)
		{
			controls::WidgetRect areaRect = drawingAreas[i].getRect();
			RECT tempRect = { areaRect.left, areaRect.top, areaRect.right, areaRect.bottom };
			InvalidateRect(_hWnd, &tempRect, NULL);
		}
	}
}

void controls::Window::updateAllDrawingAreas()
{
	int16_t size = drawingAreas.size();
	for (int16_t i = 0; i < size; ++i)
	{
		controls::WidgetRect areaRect = drawingAreas[i].getRect();
		RECT tempRect = { areaRect.left, areaRect.top, areaRect.right, areaRect.bottom };
		InvalidateRect(_hWnd, &tempRect, NULL);
	}
}

void controls::Window::updateFullWindow()
{
	RECT tempRect = { _controlRect.left, _controlRect.top, _controlRect.right, _controlRect.bottom };
	InvalidateRect(_hWnd, &tempRect, NULL);
}

void controls::Window::updateDrawingAreaSize(int16_t areaId, controls::WidgetRect newSize)
{
	getDrawingArea(areaId)->updateSize(newSize);
}

void controls::Window::updateAllDrawingAreasSize(controls::WidgetRect newSize)
{
	int16_t size = drawingAreas.size();
	for (int16_t i = 0; i < size; ++i)
	{
		drawingAreas[i].updateSize(newSize);
	}
}

float controls::Window::drawDrawingArea(HDC hdc, graphics::DrawingArea& area, int16_t algorithmType, int16_t matrixType, bool isFillingObject)
{
	area.drawAreaEdge(config::widgets::color::areaBackground);
	auto drawedTime = area.drawAreaObjects(algorithmType, matrixType, isFillingObject);

	HDC memHdc = CreateCompatibleDC(hdc);
	HBITMAP memBM = CreateBitmap(area.getWidth(), area.getHeight(), 1, sizeof(uint32_t) * 8, static_cast<void*>(area.getCanvas()));

	SelectObject(memHdc, memBM);

	BitBlt(hdc, area.getX(), area.getY(), area.getWidth(), area.getHeight(), memHdc, 0, 0, SRCCOPY);
	DeleteDC(memHdc);
	DeleteObject(memBM);
	return drawedTime;
}

float controls::Window::drawAllDrawingAreas(HDC hdc, int16_t algorithmType, int16_t matrixType, bool isFillingObject)
{
	auto drawedTime = 0;
	int16_t size = drawingAreas.size();
	for (int16_t i = 0; i < size; ++i)
	{
		drawedTime += drawDrawingArea(hdc, drawingAreas[i], algorithmType, matrixType, isFillingObject);
	}
	return drawedTime;
}

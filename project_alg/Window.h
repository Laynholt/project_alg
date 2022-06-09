#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Config.h"

// Widgets
#include "Control.h"
#include "Button.h"
#include "Combobox.h"
#include "Groupbox.h"
#include "Static.h"
#include "Edit.h"

// Graphics
#include "DrawingArea.h"

#include <vector>

namespace controls
{
	class Window : public Control
	{
	public:
		~Window();

		HWND addControl(int32_t typeId, LPCWSTR typeName, LPCWSTR text, int32_t flags, int32_t x, int32_t y, 
			int32_t width, int32_t height, HWND parentHwnd, HMENU hMenu, LPCWSTR elements[] = nullptr, int16_t numberElements = 0);

		int16_t addDrawingArea(controls::WidgetRect areaSize, uint32_t areaColor);

		void deleteDrawingArea(int16_t areaId);
		void deleteAllDrawingArea();

		void deleteWidget(HWND hWnd);
		void deleteAllWidgets();

		int16_t getWidgetType(HWND hWnd) const;

		graphics::DrawingArea* getDrawingArea(int16_t areaId);
		graphics::DrawingArea* getDrawingAreaByIndex(int16_t index);

		Control* getWidget(int32_t widgetId);
		Control* getWidget(HWND hWnd);
		Control* getWidgetByIndex(int16_t index);
		int16_t getWidgetsTypeSize(int16_t widgetType);

		int16_t getNumberOfWidgets() const;

		void setFontCurrentWidget(int32_t widgetId, HFONT hFont);
		void setFontAllWidgets(HFONT hFont);

		void updateDrawingArea(int16_t areaId);
		void updateAllDrawingAreas();
		void updateFullWindow();

		void updateDrawingAreaSize(int16_t areaId, controls::WidgetRect newSize);
		void updateAllDrawingAreasSize(controls::WidgetRect newSize);

		float drawDrawingArea(HDC hdc, graphics::DrawingArea& area, int16_t algorithmType, int16_t matrixType, bool isFillingObject);
		float drawAllDrawingAreas(HDC hdc, int16_t algorithmType, int16_t matrixType, bool isFillingObject);

	private:
		std::vector<Control*> widgets;
		std::vector<graphics::DrawingArea> drawingAreas;
	};
}
#endif
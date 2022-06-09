#ifndef _WIDGETRECT_H_
#define _WIDGETRECT_H_

#include <cstdint>

namespace controls
{
	struct WidgetRect
	{
	public:
		int32_t top, left, right, bottom, width, height;
	public:
		WidgetRect() : top(0), left(0), right(0), bottom(0), width(0), height(0) {}
		WidgetRect(int32_t _x, int32_t _y, int32_t _width, int32_t _height) : top(_y), left(_x), right(_x + _width),
			bottom(_y + _height), width(_width), height(_height) {}

		WidgetRect compress(int16_t numberPixels);
	};
}
#endif // !_WIDGETRECT_H_
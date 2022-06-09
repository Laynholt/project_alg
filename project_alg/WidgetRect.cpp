#include "WidgetRect.h"

controls::WidgetRect controls::WidgetRect::compress(int16_t numberPixels)
{
    return WidgetRect(left + numberPixels, top + numberPixels, width - 2 * numberPixels, height - 2 * numberPixels);
}

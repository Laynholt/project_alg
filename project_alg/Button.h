#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Control.h"

#define DEFAULT_BUTTON WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON
#define DEFAULT_CHECKBOX BS_CHECKBOX | WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER

namespace controls
{
	class Button : public Control
	{
	public:
		Button(int16_t controlType) { _controlType = controlType; }

		int32_t getStatus();
		void setStatus(int32_t buttonStatus);
	};
}

#endif
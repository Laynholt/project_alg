#ifndef _STATIC_H_
#define _STATIC_H_

#include "Control.h"

#define DEFAULT_STATIC WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE | BS_PUSHBUTTON

namespace controls
{
	class Static : public Control
	{
	public:
		Static() { _controlType = type::STATIC; }

		LPCWSTR getText() const;
		void setText(LPCWSTR text) const;
	};
}
#endif
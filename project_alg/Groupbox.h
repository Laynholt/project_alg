#ifndef _GROUPBOX_H_
#define _GROUPBOX_H_

#include "Control.h"

#define DEFAULT_GROUPBOX WS_VISIBLE | WS_CHILD | BS_GROUPBOX | WS_GROUP | WS_TABSTOP

namespace controls
{
	class Groupbox : public Control
	{
	public:
		Groupbox() { _controlType = type::GROUPBOX; }
	};
}

#endif
#ifndef _COMBOBOX_H_
#define _COMBOBOX_H_

#include "Control.h"

#define DEFAULT_COMBOBOX CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD \
| WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL | ES_CENTER  //| CBS_OWNERDRAWFIXED // ‘лаг того, что мы рисуем его сами

namespace controls
{
	class Combobox : public Control
	{
	public:
		Combobox() { _controlType = type::COMBOBOX; }
	
		int32_t getRow() const;
		int32_t getSize() const;
		LPCWSTR getRowText() const;

		void setRow(int32_t newRow) const;
		void addRow(LPCWSTR rowText) const;
	};
}
#endif
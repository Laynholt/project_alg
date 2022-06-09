#ifndef _EDIT_H_
#define _EDIT_H_

#include "Control.h"

#define DEFAULT_EDIT WS_VISIBLE | WS_CHILD | ES_CENTER | ES_MULTILINE | ES_READONLY

namespace controls
{
	class Edit : public Control
	{
	public:
		Edit() : digitalLimitMax(0), digitalLimitMin(0) { _controlType = type::EDIT; }

		LPCWSTR getText();
		int16_t getDigit();
		int16_t getLimitMax() const;
		int16_t getLimitMin() const;

		void setText(LPCWSTR text) const;
		void setTextLimit(int32_t limit) const;
		void setLimitMax(int16_t limit);
		void setLimitMin(int16_t limit);

	private:
		int16_t digitalLimitMax;
		int16_t digitalLimitMin;

		std::wstring text;
	};
}
#endif

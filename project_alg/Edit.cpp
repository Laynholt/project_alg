#include "Edit.h"

LPCWSTR controls::Edit::getText()
{
    WCHAR buf[5];
    Edit_GetText(_hWnd, buf, 5);
    text = buf;
    return text.c_str();
}

int16_t controls::Edit::getDigit()
{
    std::wstring buf = getText();
    if (buf == L"" || buf == L"-") return 0;
    return std::stoi(buf);
}

int16_t controls::Edit::getLimitMax() const
{
    return digitalLimitMax;
}

int16_t controls::Edit::getLimitMin() const
{
    return digitalLimitMin;
}

void controls::Edit::setText(LPCWSTR text) const
{
    Edit_SetText(_hWnd, text);
}

void controls::Edit::setTextLimit(int32_t limit) const
{
    SendMessage(_hWnd, EM_SETLIMITTEXT, limit, 0);
}

void controls::Edit::setLimitMax(int16_t limit)
{
    digitalLimitMax = limit;
}

void controls::Edit::setLimitMin(int16_t limit)
{
    digitalLimitMin = limit;
}

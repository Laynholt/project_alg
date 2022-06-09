#include "Combobox.h"

int32_t controls::Combobox::getRow() const
{
    return ComboBox_GetCurSel(_hWnd);
}

int32_t controls::Combobox::getSize() const
{
    return ComboBox_GetCount(_hWnd);
}

LPCWSTR controls::Combobox::getRowText() const
{
    WCHAR buf[100];
    ComboBox_GetLBText(_hWnd, ComboBox_GetCurSel(_hWnd), buf);
    return buf;
}

void controls::Combobox::setRow(int32_t newRow) const
{
    int32_t size = ComboBox_GetCount(_hWnd);

    if (newRow < size && newRow > -1)
        ComboBox_SetCurSel(_hWnd, newRow);
}

void controls::Combobox::addRow(LPCWSTR rowText) const
{
    ComboBox_AddString(_hWnd, rowText);
}

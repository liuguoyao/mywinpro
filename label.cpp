#include "label.h"
#include <sstream>

label::label():
  control_base()
{
}

label::label(const std::wstring &name, control_base* parent)
  :control_base(name,parent)
{

}

label::label(const control_base& other)
  :control_base(other)
{
}

label::label(const control_base&& other)
  :control_base(other)
{
}

void label::onPaint(HDC hdc)
{
  control_base::onPaint(hdc);

  point p1 = position_in_app();

  RECT rect = { p1.x, p1.y, p1.x+width, p1.y+height };

  SetTextColor(hdc, RGB(255, 0, 0));
  //SetBkColor(hdc,  RGB(0, 0, 255));
  SetBkMode(hdc, TRANSPARENT);


  DrawText(hdc, name.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER|DT_NOCLIP);

}

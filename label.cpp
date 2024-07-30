#include "label.h"

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

  point p1(x_relative_parent, y_relative_parent);
  point p2(with + x_relative_parent, height + y_relative_parent);
  point relativep = globalposition();
  p1 += relativep;
  p2 += relativep;
  RECT rect = { p1.x, p1.y, with, height };
  DrawText(hdc, name.c_str(), (int)name.size(), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  OutputDebugString(L"Label::onPaint");

}

#include "button.h"
#include "global_var.h"

using namespace std::placeholders;
button::button()
{
}

button::button(std::wstring name, control_base* parent)
  : control_base(name, parent)
{
  //this->onEnter = [&]() {
  //    setBkColor(::hoverColor);
  //  };
  //this->onLeave = [&]() {
  //  setBkColor(::backgroundColor);
  //  };
  //this->onLButtonDown = [&]() {
  //  setBkColor(::clickDownColor);
  //  };
  //this->onLButtonUp = [&]() {
  //  setBkColor(::backgroundColor);
  //  };

}

void button::onPaint(HDC hdc)
{
  control_base::onPaint(hdc);

  point p1(x_relative_parent, y_relative_parent);
  p1 += position_in_app();
  RECT rect = { p1.x, p1.y, p1.x + width, p1.y + height };

  SetTextColor(hdc, RGB(fontColor.r, fontColor.g, fontColor.b));
  //SetBkColor(hdc,  RGB(0, 0, 255));
  SetBkMode(hdc, TRANSPARENT);

  DrawText(hdc, name.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
}

void button::processMouseEnter()
{
  control_base::processMouseEnter();
  setBkColor(::hoverColor);
}
void button::processMouseLeave()
{
  control_base::processMouseLeave();
  setBkColor(::backgroundColor);
}
void button::onupdateAnimState(long long delta_time)
{
  control_base::onupdateAnimState(delta_time);
  if (hover) {
    
  }
}

void button::processLButtonDown()
{
  control_base::processLButtonDown();
  setBkColor(::clickDownColor);
}

void button::processLButtonUp()
{
  control_base::processLButtonUp();
  hover?setBkColor(::hoverColor):setBkColor(::backgroundColor);
}

void button::processLButtonDBLClick()
{
  control_base::processLButtonDBLClick();

}


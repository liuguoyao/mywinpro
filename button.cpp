#include "button.h"
#include "global_var.h"

using namespace std::placeholders;
button::button()
{
}

button::button(std::wstring name, control_base* parent)
  : control_base(name, parent),_text(name)
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
  point p1 = position_in_app();

  RECT rect = { p1.x, p1.y, p1.x + width, p1.y + height };

  SetTextColor(hdc, RGB(fontColor.r, fontColor.g, fontColor.b));
  //SetBkColor(hdc,  RGB(0, 0, 255));
  SetBkMode(hdc, TRANSPARENT);

  DrawText(hdc, _text.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
}

void button::processMouseEnter(evt e)
{
  control_base::processMouseEnter(e);
  setBkColor(::hoverColor);
}
void button::processMouseLeave(evt e)
{
  control_base::processMouseLeave(e);
  setBkColor(::backgroundColor);
}
std::wstring button::get_text()
{
  return _text;
}
void button::set_text(std::wstring text)
{
  _text = text;
}
void button::onupdateAnimState(long long delta_time)
{
  control_base::onupdateAnimState(delta_time);
  if (hover) {
    
  }
}

void button::processLButtonDown(evt e)
{
  control_base::processLButtonDown(e);
  setBkColor(::clickDownColor);
}

void button::processLButtonUp(evt e)
{
  control_base::processLButtonUp(e);
  hover?setBkColor(::hoverColor):setBkColor(::backgroundColor);
}

void button::processLButtonDBLClick(evt e)
{
  control_base::processLButtonDBLClick(e);

}


#include "edit.h"

edit::edit(std::wstring name, control_base* parent)
  :control_base(name, parent), _time_acc(0), _draw_text_cursor(true)
{
  _sizePolicy.xPolicy = SIZEPOLICY_EXPAND;
}

void edit::onPaint(HDC hdc)
{
  control_base::onPaint(hdc);

  std::wstring text = _context+_comtext;
  SIZE text_size = { 0, 0 };
  GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &text_size);

  point p1 = position_in_app();

  RECT rect = { p1.x, p1.y, p1.x + width, p1.y + height };
  DrawText(hdc, text.c_str(), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER );

  //  draw �ı����
  if(_draw_text_cursor && hasFocus())
  {
    int cursor_margin = 4;
    MoveToEx(hdc, p1.x+ cursor_margin+ text_size.cx, p1.y+ cursor_margin, NULL);
    LineTo(hdc, p1.x+ cursor_margin + text_size.cx, p1.y+height- cursor_margin);
  }

}

void edit::onupdateAnimState(long long delta_time)
{
  control_base::onupdateAnimState(delta_time);
  _time_acc+=(unsigned short)delta_time;
  if(_time_acc>500)
  {
    _draw_text_cursor = !_draw_text_cursor;
    _time_acc = 0;
  }
  
}

void edit::processLButtonDown(evt e)
{
  control_base::processLButtonDown(e);
  point pinc = position_in_app();
  point p = point(e.x,e.y);
}

void edit::processLButtonUp(evt e)
{
  control_base::processLButtonUp(e);
}

void edit::processLButtonDBLClick(evt e)
{
  control_base::processLButtonDBLClick(e);
}

void edit::processMouseEnter(evt e)
{
  control_base::processMouseEnter(e);
}

void edit::processMouseLeave(evt e)
{
  control_base::processMouseLeave(e);
}

void edit::setFocus(bool focus)
{
  control_base::setFocus(focus);
  if (focus)
  {
    _draw_text_cursor= true;
    _time_acc = 0;
  }
}

void edit::set_text(const std::wstring& text)
{
  _context = text;
}

std::wstring edit::get_text() const
{
  return _context;
}

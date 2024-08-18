#include "edit.h"
#include "app.h"

edit::edit(std::wstring name, control_base* parent)
  :control_base(name, parent), _time_acc(0), _draw_text_cursor(true)
{
  _sizePolicy.xPolicy = SIZEPOLICY_EXPAND;
}

void edit::onPaint(HDC hdc)
{
  control_base::onPaint(hdc);

  std::wstring text = _context+_comtext;

  point p1 = position_in_app();
  RECT rect = { p1.x, p1.y, p1.x + width, p1.y + height };
  DrawText(hdc, text.c_str(), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER );

  //  draw �ı����
  if(_draw_text_cursor && hasFocus())
  {
    int cursor_margin = 0;
    MoveToEx(hdc, p1.x+ cursor_margin+ _pos_text_cursor, p1.y+ cursor_margin, NULL);
    LineTo(hdc, p1.x+ cursor_margin + _pos_text_cursor, p1.y+height- cursor_margin);
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
  point pinc = point(e.x, e.y);
  pinc -= position_in_app();
  pinc.x -= 4;
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
  _pos_text_cursor = text_width(APP.hWnd, _context + _comtext);
}

std::wstring edit::get_text() const
{
  return _context;
}

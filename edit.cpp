#include "edit.h"
#include "app.h"
#include <cwctype>

edit::edit(std::wstring name, control_base* parent)
  :control_base(name, parent), _time_acc(0), _draw_text_cursor(true),
  _pos_text_cursor(0.0),_text_selectd_start_pos(0.0), _text_selectd_end_pos(0.0)
{
  _sizePolicy.xPolicy = SIZEPOLICY_EXPAND;
}

void edit::onPaint(HDC hdc)
{
  control_base::onPaint(hdc);

  std::wstring text = _context+_comtext;
  point p1 = position_in_app();

  // 选中背景
  if (abs(_text_selectd_start_pos - _text_selectd_end_pos) > 1){
    RECT rect_selected = { p1.x + _text_selectd_start_pos, p1.y + 4, p1.x + _text_selectd_end_pos, p1.y + height - 4 };
    HBRUSH hbr = CreateSolidBrush(RGB(textSelectedBgColor.r, textSelectedBgColor.g, textSelectedBgColor.b)); // 白色背景  
    FillRect(hdc, &rect_selected, hbr);
    DeleteObject(hbr);
  }

  RECT rect = { p1.x, p1.y, p1.x + width, p1.y + height };
  DrawText(hdc, text.c_str(), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER );

  //  draw 文本光标
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

  _time_acc = 0;
  _draw_text_cursor = true;
  if (_context.length() > 0)
  {
    point pinc = point(e.x, e.y);
    pinc -= position_in_app();
    float w = text_width(APP.hWnd, _context);
    float char_w = w / _context.length();
    int prefer_pos_text_cursor = char_w * round(pinc.x / char_w);
    _pos_text_cursor = prefer_pos_text_cursor > w ? w : prefer_pos_text_cursor;
    _text_selectd_start_pos = _pos_text_cursor;
    _text_selectd_end_pos = _pos_text_cursor;
  }
  else
  {
    _text_selectd_start_pos = _pos_text_cursor = _text_selectd_end_pos = 0;
  }
  
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

void edit::processMouseMove(const point& p)
{
  control_base::processMouseMove(p);

  if (hasFocus() && mouseLeftButtonDown)
  {
    _time_acc = 0;
    _draw_text_cursor = true;
    point pinc = p;
    pinc -= position_in_app();
    float w = text_width(APP.hWnd, _context);
    float char_w = w / _context.length();
    int prefer_pos_text_cursor = char_w * round(pinc.x / char_w);
    _pos_text_cursor = prefer_pos_text_cursor > w ? w : prefer_pos_text_cursor;
    _pos_text_cursor = _pos_text_cursor < 0 ? 0 : _pos_text_cursor;
    _text_selectd_end_pos = _pos_text_cursor;
  }
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

void edit::processIMMEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  control_base::processIMMEvent(hWnd, message, wParam, lParam);

  if (hasFocus())
  {
    switch (message)
    {  //imm
    case WM_CHAR:
      if (wParam == VK_BACK) {
        if (_context.length() > 0) {
          float w = text_width(APP.hWnd, _context);
          float char_w = w / _context.length();
          if (abs(_text_selectd_start_pos - _text_selectd_end_pos) < 1) {
            _context.erase(round(_pos_text_cursor / char_w)-1,1);
            _pos_text_cursor = _text_selectd_start_pos = _text_selectd_end_pos = _pos_text_cursor - char_w;
          }
          else {
            int begin = min(_text_selectd_start_pos , _text_selectd_end_pos)/ char_w;
            int end = max(_text_selectd_start_pos , _text_selectd_end_pos)/ char_w;
            _context.erase(begin, end - begin);
            _pos_text_cursor = _text_selectd_start_pos = _text_selectd_end_pos = round(begin* char_w);
            OutputDebugString((L"_pos_text_cursor:"+std::to_wstring(_text_selectd_start_pos)+L"\n").c_str());
          }
        }
        break;
      }
      else if (wParam == VK_ESCAPE) {
        _context.clear();
        _pos_text_cursor = 1;
        break;
      }
      else if (std::iswprint((unsigned short)wParam))
      {
        _context += (WCHAR)wParam;
        _pos_text_cursor = text_width(hWnd, _context + _comtext);
      }
      else
      {

      }
      break;
    case WM_IME_STARTCOMPOSITION: {
      if (!hasFocus()) break;
      HIMC hIMC = ImmGetContext(hWnd);
      if (hIMC) {
        COMPOSITIONFORM cf;
        cf.dwStyle = CFS_POINT; // 使用点定位
        POINT pt = { position_in_app().x * 2, position_in_app().y + height };
        cf.ptCurrentPos = pt;   // 设置候选框的位置
        ImmSetCompositionWindow(hIMC, &cf);

        ImmReleaseContext(hWnd, hIMC);
      }
    }
                                break;
    case WM_IME_COMPOSITION: {
      if (!hasFocus()) break;
      WCHAR szCompStr[256];
      HIMC hIMC = ImmGetContext(hWnd);
      memset(szCompStr, 0, sizeof(szCompStr));
      if (lParam & GCS_COMPSTR) {
        DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, szCompStr, 256);
        _comtext = szCompStr;
      }
      else if (lParam & GCS_RESULTSTR)
      {
        LONG buflen = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);
        ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, szCompStr, buflen);
        _comtext = L"";
        _context += szCompStr;
      }
      _pos_text_cursor = text_width(hWnd, _context + _comtext);
      _text_selectd_start_pos = _text_selectd_end_pos = _pos_text_cursor;

      // 更新文本框显示，处理dwSize字节的输入字符串
      OutputDebugString(szCompStr);
      ImmReleaseContext(hWnd, hIMC);
      InvalidateRect(hWnd, NULL, false);
      break;
    }
    case WM_IME_ENDCOMPOSITION:
      break;

    default:
      //DefWindowProc(hWnd, message, wParam, lParam);
      break;
    }
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

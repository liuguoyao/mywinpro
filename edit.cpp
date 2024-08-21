#include "edit.h"
#include "app.h"
#include "util.h"
#include <cwctype>

std::wstring get_clipboard_text()
{
  std::wstring clipboardText;

  if (OpenClipboard(NULL))
  {
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData)
    {
      const wchar_t* pszText = (const wchar_t*)GlobalLock(hData);
      if (pszText)
      {
        clipboardText = pszText;
        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
  return clipboardText;
}
int copy_to_clipboard(std::wstring text)
{
  if (!OpenClipboard(NULL))
  {
    return 1;
  }
  EmptyClipboard();

  HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length()+1)*2);
  if (hMem == NULL)
  {
    CloseClipboard();
    return 1;
  }
  LPTSTR lpMem = (LPTSTR)GlobalLock(hMem);
  lstrcpy(lpMem, text.c_str());
  GlobalUnlock(hMem);
  SetClipboardData(CF_UNICODETEXT, hMem);
  CloseClipboard();
  return 0;
}

edit::edit(std::wstring name, control_base* parent)
  :control_base(name, parent), _time_acc(0), _draw_text_cursor(true),
  _pos_text_cursor(0.0),_text_selectd_start_pos(0.0), _text_selectd_end_pos(0.0),
  char_w(control_base::text_width(APP.hWnd, L"A"))
{
  _sizePolicy.xPolicy = SIZEPOLICY_EXPAND;
}

void edit::onPaint(HDC hdc)
{
  control_base::onPaint(hdc);

  point p1 = position_in_app();

  std::wstring text = _context;
  if (_comtext.length()>0)
  {
    int left = 0, right = 0;
    getSelected(left, right);
    text.replace(left, 0, _comtext);
    _text_selectd_end_pos = text_width(_comtext) + text_width(text.substr(0, left));
 
  }

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

  point pinc = point(e.x, e.y);
  pinc -= position_in_app();
  auto isAsii = checkAsciiInWString(_context);
  setTextCusor(isAsii.size());
  int len = 0;
  for (int i=0;i<isAsii.size();i++)
  {
    len += char_w;
    if (!isAsii[i])
      len += char_w;
    if (pinc.x < len) {
      setTextCusor(i);
      break;
    }
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

    float w = text_width(_context);

    if (abs(_text_selectd_end_pos - pinc.x)>=char_w)
    {
      auto isAsii = checkAsciiInWString(_context);

      int len = 0;
      for (int i = 0; i < isAsii.size(); i++)
      {
        len += char_w;
        if (!isAsii[i])
          len += char_w;
        if (pinc.x < len) {
          _text_selectd_end_pos = len;
          break;
        }
      }
    }

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
    {  
    case WM_KEYDOWN:
      if (GetAsyncKeyState(VK_CONTROL))
      {
        if (wParam == 'C') { // 检查是否为 CTRL+C
          if (_context.length() > 0)
          {
            int left=0, right = 0;
            getSelected(left,right);
            copy_to_clipboard(_context.substr(left, right-left));
          }
        }
        else if (wParam == 'V') { // 检查是否为 CTRL+V
          std::wstring clipboardText = get_clipboard_text();
          int left = 0, right = 0;
          getSelected(left, right);
          _context.replace(left, right - left, clipboardText);
          setTextCusor(left + clipboardText.length());
        }
        else if (wParam == 'X') { // 检查是否为 CTRL+X
          int left = 0, right = 0;
          getSelected(left, right);
          if (left < right)
          {
            copy_to_clipboard(_context.substr(left, right - left));
            _context.erase(left, right - left);
            setTextCusor(left);
          }
        }
      }
      if (wParam == VK_LEFT)
      {
        _time_acc = 0;
        _draw_text_cursor = true;
        textCusorShift(false, GetAsyncKeyState(VK_SHIFT));
      }
      if (wParam == VK_RIGHT)
      {
        _time_acc = 0;
        _draw_text_cursor = true;
        textCusorShift(true, GetAsyncKeyState(VK_SHIFT));
      }
      break;
      //imm
    case WM_CHAR:
      if (wParam == VK_BACK) {
        if (_context.length() > 0) {
          int left = 0, right = 0;
          getSelected(left, right);
          if (left!=right)
          {
            _context.erase(left, right - left);
            setTextCusor(left);
          }
          else {
            if(left>0)
            _context.erase(left - 1, 1);
            setTextCusor(left-1);
          }
        }
        break;
      }
      else if (wParam == VK_ESCAPE) {
        _context.clear();
        setTextCusor(0);
        clearSelected();
        break;
      }
      else if (std::iswprint((unsigned short)wParam))
      {
        int left = 0, right = 0;
        getSelected(left, right);
        std::wstring in_text ;
        in_text = (WCHAR)wParam;
        _context.replace(left, right-left, in_text);
        textCusorShift(true);
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

        std::wstring in_text = szCompStr;
        int left = 0, right = 0;
        getSelected(left, right);
        _context.replace(left, 0, in_text);
        setTextCusor(left+in_text.length());
      }

      // 更新文本框显示，处理dwSize字节的输入字符串
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

bool edit::processEvent(evt e)
{
  control_base::processEvent(e);
  return false;
}

void edit::set_text(const std::wstring& text)
{
  _context = text;
  _pos_text_cursor = text_width(_context + _comtext);
}

std::wstring edit::get_text() const
{
  return _context;
}

void edit::setSelected(int left, int right)
{
  left = left < 0 ? 0 : left;
  right = right > _context.length() ? _context.length() : right;
  auto isAsii_v = checkAsciiInWString(_context);

  _text_selectd_start_pos = 0;
  for (short i = 0; i < left; i++)
  {
    if (isAsii_v[i])
      _text_selectd_start_pos += char_w;
    else
      _text_selectd_start_pos += char_w*2;
  }

  _text_selectd_end_pos = _text_selectd_start_pos;
  for (short i = left; i < right; i++)
  {
    if (isAsii_v[i])
      _text_selectd_end_pos += char_w;
    else
      _text_selectd_end_pos += char_w * 2;
  }
  _pos_text_cursor = _text_selectd_end_pos;
}

void edit::getSelected(int& left, int& right)
{
  auto isAsii_v = checkAsciiInWString(_context);
  int len = 0;
  float start = _text_selectd_start_pos;
  float end = _text_selectd_end_pos;
  if (start>end)
  {
    end = _text_selectd_start_pos;
    start = _text_selectd_end_pos;
  }

  if (len >= start) left = 0;
  if (len >= end) right = 0;

  if (0 == start) {
    left = 0;
  }
  else {
    for (short i = 0; i < _context.length(); i++)
    {
      if (isAsii_v[i])
        len += char_w;
      else
        len += char_w * 2;

      if (len >= start) {
        left = i + 1;
        break;
      }
    }
  }

  if (len == end) {
    right = left;
  }
  else {
    for (short i = left; i < _context.length(); i++)
    {
      if (isAsii_v[i])
        len += char_w;
      else
        len += char_w * 2;

      if (len >= end) {
        right = i + 1;
        break;
      }
    }
  }
}

void edit::clearSelected()
{
  _text_selectd_end_pos = _text_selectd_start_pos = _pos_text_cursor;
}


void edit::textCusorShift(bool forward, bool select_flag)
{
  auto isAsii_v = checkAsciiInWString(_context);
  int cnt = 0;
  getTextCusor(cnt);

  if (forward)
  {
    if (cnt >= _context.length()) return;
  }
  else {
    if (cnt <= 0 ) return;
  }

  int step = forward ? char_w : -char_w;
  cnt = forward ? cnt : cnt - 1;

  _pos_text_cursor += step;
  if (!isAsii_v[cnt])
    _pos_text_cursor += step;
  
  _text_selectd_end_pos = _pos_text_cursor;

  if (!select_flag)
    _text_selectd_start_pos = _text_selectd_end_pos;
}

void edit::setTextCusor(int cnt)
{
  auto isAsii_v = checkAsciiInWString(_context);
  int len = 0;
  if (cnt > _context.length()) cnt = _context.length();
  if (cnt < 0) cnt = 0;

  for (short i = 0; i < cnt; i++)
  {
    if (isAsii_v[i])
      len += char_w;
    else
      len += char_w * 2;
  }
  _pos_text_cursor = _text_selectd_start_pos = _text_selectd_end_pos = len;
}

void edit::getTextCusor(int &cnt)
{
  auto isAsii_v = checkAsciiInWString(_context);
  int len = 0;
  if (len >= _pos_text_cursor) cnt = 0;
  if (0 == _pos_text_cursor) {
    cnt = 0;
    return;
  }

  for (short i = 0; i < _context.length(); i++)
  {
    if (isAsii_v[i])
      len += char_w;
    else
      len += char_w * 2;

    if (len >= _pos_text_cursor) {
      cnt = i + 1;
      break;
    }
  }
}

int edit::text_width(const std::wstring& text)
{
  auto isAsii = checkAsciiInWString(text);
  int len = 0;
  for (auto c : isAsii)
  {
    len += char_w;
    if (!c)
      len += char_w;
  }
  return len;
}

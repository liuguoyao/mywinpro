#include "control_base.h"
#include "structs.h"
#include "global_var.h"
#include "app.h"
#include<limits>
#include <sstream>
#include <imm.h>
#include <cwctype>


control_base::control_base(const std::wstring &name, control_base* parent) :
  x_relative_parent(0),
  y_relative_parent(0),
  height(30),
  width(60),
  parent(parent),
  hover(false),
  onMouseMove([](const point &) {}),
  onClick([](evt) {}),
  onDoubleClick([](evt) {}),
  onEnter([](evt) {}),
  onLeave([](evt) {}),
  onLButtonDown([](evt) {}),
  onLButtonUp([](evt) {}),
  onLButtonDBLClick([](evt) {}),
  name(name),
  id(0),
  needupdate(true),
  borderColor(::borderColor),
  hoverColor(::hoverColor),
  activeColor(::activeColor),
  backgroundColor(::backgroundColor),
  fontColor(::fontColor),
  curborderColor(::borderColor),
  curhoverColor(::hoverColor),
  curactiveColor(::activeColor),
  curbackgroundColor(::backgroundColor),
  curfontColor(::fontColor),
  deltaborderColor(0,0,0),
  deltahoverColor(0, 0, 0),
  deltaactiveColor(0, 0, 0),
  deltabackgroundColor(0, 0, 0),
  deltafontColor(0, 0, 0),
  mouseLeftButtonDown(false),
  mouseRightButtonDown(false),
  mouseLeftButtonUp(false),
  mouseRightButtonUp(false),
  mouseLeftButtonClick(false),
  _hasFocus(false),
  _sizePolicy{SIZEPOLICY_FIXED,SIZEPOLICY_FIXED,0,0}
{
  if (nullptr != parent)
  {
    parent->addChild(this);
  }
}

control_base::~control_base()
{
  std::wostringstream o;
  o << "~ " << this << " " << this->x_relative_parent << "\n";
}

std::wstring control_base::classtype() const
{
    return L"control_base";
}

void control_base::paint(HDC hdc)
{

  onPaint(hdc);
  for (auto &c:childrens )
  {
    c->paint(hdc);
  }

}

void control_base::resize(int w, int h)
{
  width = w;
  height = h;
}

void control_base::resize(size s)
{
  resize(s.x, s.y);
}

size control_base::getSize()
{
    return size(width,height);
}

void control_base::setposition(int x, int y)
{
  x_relative_parent = x;
  y_relative_parent = y;
}

point control_base::position()
{
  return point(x_relative_parent, y_relative_parent);
}

point control_base::position_in_app()
{
  control_base* cur_p = parent;
  point curp = position();
  while (nullptr!=cur_p)
  { 
    curp += cur_p->position();
    cur_p = cur_p->parent;
  }
  return curp;
}

control_base* control_base::addChild(control_base* control)
{
  std::pair<std::set<control_base*>::iterator, bool> pair = childrens.emplace(control);
  if (pair.second)
  {
    return (control_base*)  *pair.first;
  }
  else {
    MessageBox(NULL, L"has same name of control", L"", MB_OK);
  }
  return nullptr;
}

bool control_base::containsPoint(const point& p)
{
  point p1(0, 0);
  point p2(width , height );
  point relativep = position_in_app();
  p1 += relativep;
  p2 += relativep;
  if (p.x<p2.x && p.x>p1.x && p.y>p1.y && p.y<p2.y)
  {
    return true;
  }
  return false;
}

std::vector<control_base*> control_base::controlsAtPoint(const point& p)
{
  std::vector<control_base*> ret;
  if (containsPoint(p))
  {
    ret.push_back(this);
  }
  for (auto &c:childrens)
  {
    //auto child_ret = ((control_base&)c).controlsAtPoint(p);
    auto child_ret = c->controlsAtPoint(p);
    for (auto c2:child_ret)
    {
      ret.push_back(c2);
    }
  }

  return ret;
}

bool control_base::processEvent(evt e)
{
  for (auto c : childrens)
  {
    //if(c->processEvent(e)) return true;
    c->processEvent(e);
  }

  if (!containsPoint(point(e.x, e.y))
    && e.type != WM_MOUSEMOVE
    && e.type != WM_LBUTTONUP
    ) {

    return false;
  }
  switch (e.type)
  {
  case WM_MOUSEMOVE:
    if(!mouseLeftButtonDown)
      processMouseMove(point(e.x,e.y));
    break;
  case WM_LBUTTONDOWN:
    mouseLeftButtonDown=true;
    processLButtonDown(e);
    break;
  case WM_LBUTTONUP:
    if (mouseLeftButtonDown)
    {
      mouseLeftButtonDown = false;
      processLButtonUp(e);
    }
    break;
  case WM_LBUTTONDBLCLK:
    processLButtonDBLClick(e);
    break;
  default:
    break;
  }
  return false;
}

void control_base::processMouseMove(const point& global_p)
{
  
  if (containsPoint(global_p))
  {
    if (!hover)
    {
      hover = true;
      processMouseEnter(evt(global_p.x, global_p.y,0,0) );
    }
  }
  else
  {
    if (hover)
    {
      if (!mouseLeftButtonDown) {
        hover = false;
        processMouseLeave(evt(global_p.x, global_p.y, 0, 0));
      }
        
    }
  }
  for (const auto& c2 : childrens)
  {
    c2->processMouseMove(global_p);
  }
  needupdate = true;
}

void control_base::processMouseEnter(evt e)
{
  onEnter(e);
}

void control_base::processMouseLeave(evt e)
{
  onLeave(e);
}

void control_base::processLButtonDown(evt e)
{
  setFocus(true);
  PostMessage(APP.hWnd, WM_MY_SETFOCUS, (WPARAM)this, 0);
  onLButtonDown(e);
}

void control_base::processLButtonUp(evt e)
{
  onLButtonUp(e);
}

void control_base::processLButtonDBLClick(evt e)
{
  onLButtonDBLClick(e);
}

void control_base::updateState(long long delt_time)
{
  onupdateAnimState(delt_time);

  if (needupdate) invalidate();

  for (const auto& c2 : childrens)
  {
    c2->updateState(delt_time);
  }
}

void control_base::setBkColor(const rgb& bkcolor)
{
  backgroundColor = bkcolor;
  deltabackgroundColor = (backgroundColor - curbackgroundColor)/3.0f;
}

void control_base::onPaint(HDC hdc)
{
  point p1(0, 0);
  point p2(width , height );
  point relativep = position_in_app();
  p1 += relativep;
  p2 += relativep;

  HPEN hPen; 
  if (hasFocus())
  {
    hPen = CreatePen(PS_SOLID, 1, RGB(focusBorderColor.r, focusBorderColor.g, focusBorderColor.b));
  }
  else
  {
    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  }
  
  rgb color;
  HBRUSH hBrush = CreateSolidBrush(RGB(curbackgroundColor.r, curbackgroundColor.g, curbackgroundColor.b));
  HGDIOBJ oldPen = SelectObject(hdc, hPen);
  HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

  // 绘制矩形，这里假设是填充的矩形  
  Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);
  // 恢复旧的画笔和笔刷  
  SelectObject(hdc, oldPen);
  SelectObject(hdc, oldBrush);

  // 删除创建的笔刷和画笔  
  DeleteObject(hPen);
  DeleteObject(hBrush);

}

void control_base::onupdateAnimState(long long delt_time)
{
  if (deltabackgroundColor.abs() > FLT_MIN) {
    curbackgroundColor += deltabackgroundColor;
    if ((curbackgroundColor - backgroundColor).abs() < 0.1f)
    {
      curbackgroundColor = backgroundColor;
      deltabackgroundColor = rgb(0,0,0);
    }
    needupdate = true;
  }
}

void control_base::processIMMEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (hasFocus())
  {
    switch (message)
    {  //imm
    case WM_CHAR:
      if (wParam == VK_BACK) {
        if(_context.length()>0) _context.erase(_context.length() - 1);
        break;
      }else if (wParam == VK_ESCAPE) {
        _context.clear();
        break;
      }
      else if(std::iswprint((unsigned short)wParam))
      {
        _context += (WCHAR)wParam;
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

  for (auto c:childrens)
  {
    c->processIMMEvent(hWnd,  message,  wParam,  lParam);
  }
}

bool control_base::hasFocus()
{
  return _hasFocus;
}

void control_base::setFocus(bool focus)
{
  _hasFocus = focus;
}


bool control_base::operator<(const control_base &other) const
{
  return this->name<other.name;
}

sizePolicy control_base::getSizePolicy() const
{
  return _sizePolicy;
}

void control_base::setSizePolicy(sizePolicy& sp)
{
    _sizePolicy = sp;
}

void control_base::setxPolicy(int xPolicy)
{
  _sizePolicy.xPolicy = xPolicy;
}

void control_base::setyPolicy(int yPolicy)
{
  _sizePolicy.yPolicy = yPolicy;
}

void control_base::setxFactor(int xFactor)
{
  _sizePolicy.xFactor = xFactor;
}

void control_base::setyFactor(int yFactor)
{
  _sizePolicy.yFactor = yFactor;
}

void control_base::invalidate()
{
  RECT rect = rect_global();
  InvalidateRect(app::getInstance().hWnd, &rect, false);
}

RECT control_base::rect_global()
{
  point p1 = position_in_app();

  RECT rect = { p1.x, p1.y, p1.x + width, p1.y + height };
  return rect;
}

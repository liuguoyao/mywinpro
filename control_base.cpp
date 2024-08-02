#include "control_base.h"
#include "structs.h"
#include "global_var.h"
#include "app.h"
#include<limits>
#include <sstream>

control_base::control_base() :
  x_relative_parent(0),
  y_relative_parent(0),
  height(0),
  with(0),
  parent(nullptr),
  hover(false),
    onClick([]() {}),
    onDoubleClick([]() {}),
    onEnter([]() {}),
    onLeave([]() {}),
  name(L""),
  id(0)
{
}

control_base::control_base(const std::wstring &name, control_base* parent) :
  x_relative_parent(0),
  y_relative_parent(0),
  height(0),
  with(0),
  parent(parent),
  hover(false),
  onClick([]() {}),
  onDoubleClick([]() {}),
  onEnter([]() {}),
  onLeave([]() {}),
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
  deltafontColor(0, 0, 0)
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
  with = w;
  height = h;
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

point control_base::globalposition()
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
  point p1(x_relative_parent, y_relative_parent);
  point p2(with + x_relative_parent, height + y_relative_parent);
  point relativep = globalposition();
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

void control_base::updateMousePosition(const point& global_p)
{
  
  if (containsPoint(global_p))
  {
    if (!hover)
    {
      OutputDebugString((L"onEnter " + name + L" \n").c_str());
      hover = true;
      onEnter();
    }
  }
  else
  {
    if (hover)
    {
      hover = false;
      onLeave();
    }
  }
  for (const auto& c2 : childrens)
  {
    c2->updateMousePosition(global_p);
  }
  needupdate = true;
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
  point p1(x_relative_parent, y_relative_parent);
  point p2(with + x_relative_parent, height + y_relative_parent);
  point relativep = globalposition();
  p1 += relativep;
  p2 += relativep;

  HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
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
    if ((curbackgroundColor-backgroundColor).abs()<FLT_MIN)
    {
      deltabackgroundColor = rgb(0,0,0);
    }
    needupdate = true;
  }
}


bool control_base::operator<(const control_base &other) const
{
  return this->name<other.name;
}

void control_base::invalidate()
{
  RECT rect = rect_global();
  InvalidateRect(app::getInstance().hWnd, &rect, false);
}

RECT control_base::rect_global()
{
  point p1(x_relative_parent, y_relative_parent);
  p1 += globalposition();
  RECT rect = { p1.x, p1.y, p1.x + with, p1.y + height };
  return rect;
}

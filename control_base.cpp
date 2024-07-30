#include "control_base.h"
#include "structs.h"
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

control_base::control_base(const std::wstring &name) :
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
  name(name),
  id(0)
{
}

control_base::control_base(const control_base& ctrl_base)
{
  this->childrens = ctrl_base.childrens;
  this->parent = ctrl_base.parent;
  this->x_relative_parent = ctrl_base.x_relative_parent;
  this->y_relative_parent = ctrl_base.y_relative_parent;
  this->with = ctrl_base.with;
  this->height = ctrl_base.height;
  this->hover = ctrl_base.hover;
  this->onClick = ctrl_base.onClick;
  this->onDoubleClick = ctrl_base.onDoubleClick;
  this->onEnter = ctrl_base.onEnter;
  this->onLeave = ctrl_base.onLeave;
  this->name = ctrl_base.name;
  this->id = ctrl_base.id;
}

control_base::control_base(const control_base&& ctrl_base)
{
  this->childrens = ctrl_base.childrens;
  this->parent = ctrl_base.parent;
  this->x_relative_parent = ctrl_base.x_relative_parent;
  this->y_relative_parent = ctrl_base.y_relative_parent;
  this->with = ctrl_base.with;
  this->height = ctrl_base.height;
  this->hover = ctrl_base.hover;
  this->onClick = ctrl_base.onClick;
  this->onDoubleClick = ctrl_base.onDoubleClick;
  this->onEnter = ctrl_base.onEnter;
  this->onLeave = ctrl_base.onLeave;
  this->name = ctrl_base.name;
  this->id = ctrl_base.id;
}

control_base& control_base::operator=(control_base& ctrl_base)
{
  this->childrens = ctrl_base.childrens;
  this->parent = ctrl_base.parent;
  this->x_relative_parent = ctrl_base.x_relative_parent;
  this->y_relative_parent = ctrl_base.y_relative_parent;
  this->with = ctrl_base.with;
  this->height = ctrl_base.height;
  this->hover = ctrl_base.hover;
  this->onClick = ctrl_base.onClick;
  this->onDoubleClick = ctrl_base.onDoubleClick;
  this->onEnter = ctrl_base.onEnter;
  this->onLeave = ctrl_base.onLeave;
  this->name = ctrl_base.name;
  this->id = ctrl_base.id;
  
  // TODO: �ڴ˴����� return ���
  return *this;
}

control_base& control_base::operator=(control_base&& ctrl_base)
{
  this->childrens = ctrl_base.childrens;
  this->parent = ctrl_base.parent;
  this->x_relative_parent = ctrl_base.x_relative_parent;
  this->y_relative_parent = ctrl_base.y_relative_parent;
  this->with = ctrl_base.with;
  this->height = ctrl_base.height;
  this->hover = ctrl_base.hover;
  this->onClick = ctrl_base.onLeave;
  this->onDoubleClick = ctrl_base.onLeave;
  this->onEnter = ctrl_base.onLeave;
  this->onLeave = ctrl_base.onLeave;
  this->name = ctrl_base.name;
  this->id = ctrl_base.id;
  // TODO: �ڴ˴����� return ���
  return *this;
}

control_base::~control_base()
{
  std::wostringstream o;
  o << "~ " << this << " " << this->x_relative_parent << "\n";
}

void control_base::paint(HDC hdc, control_base* parent)
{
  if (parent) { 
    parent->onPaint(hdc); 
  }
  else
  {
    onPaint(hdc);
  }
  
  for (auto &c:childrens )
  {
    ((control_base&)c).paint(hdc, parent);
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

control_base* control_base::addChild(control_base& control)
{
  //childrens.push_back(control);
  //auto& c = childrens.back();
  //c.parent = this;
  //return &childrens.back();
  std::pair<std::set<control_base>::iterator, bool> pair = childrens.emplace(control);
  if (pair.second)
  {
    return (control_base*) & *pair.first;
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
    auto child_ret = ((control_base&)c).controlsAtPoint(p);
    for (auto c2:child_ret)
    {
      ret.push_back(c2);
    }
  }

  return ret;
}

void control_base::updateState(const point& global_p)
{
  if (containsPoint(global_p))
  {
    if (!hover)
    {
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
    ((control_base&)c2).updateState(global_p);
  }
}



void control_base::setBkColor(const rgb& rgb)
{
  bkrgb = rgb;
}

void control_base::onPaint(HDC hdc)
{
  point p1(x_relative_parent, y_relative_parent);
  point p2(with + x_relative_parent, height + y_relative_parent);
  point relativep = globalposition();
  p1 += relativep;
  p2 += relativep;
  //Rectangle(hdc,p1.x,p1.y , p2.x, p2.y);

  HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  rgb color;
  HBRUSH hBrush = CreateSolidBrush(RGB(bkrgb.r, bkrgb.g, bkrgb.b));
  HGDIOBJ oldPen = SelectObject(hdc, hPen);
  HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

  // ���ƾ��Σ�������������ľ���  
  Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);
  // �ָ��ɵĻ��ʺͱ�ˢ  
  SelectObject(hdc, oldPen);
  SelectObject(hdc, oldBrush);

  // ɾ�������ı�ˢ�ͻ���  
  DeleteObject(hPen);
  DeleteObject(hBrush);

}


bool control_base::operator<(const control_base &other) const
{
  return this->name<other.name;
}

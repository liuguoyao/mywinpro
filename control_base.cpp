#include "control_base.h"
#include "structs.h"
#include <sstream>

control_base::control_base() :
  x_relative_parent(0),
  y_relative_parent(0),
  height(0),
  with(0),
  parent(nullptr)
{
}

control_base::control_base(control_base& ctrl_base) 
{
  this->childrens = ctrl_base.childrens;
  this->parent = ctrl_base.parent;
  this->x_relative_parent = ctrl_base.x_relative_parent;
  this->y_relative_parent = ctrl_base.y_relative_parent;
  this->with = ctrl_base.with;
  this->height = ctrl_base.height;
  OutputDebugString(L"call copy construct\n");
}

control_base::control_base(control_base&& ctrl_base) 
{
  this->childrens = ctrl_base.childrens;
  this->parent = ctrl_base.parent;
  this->x_relative_parent = ctrl_base.x_relative_parent;
  this->y_relative_parent = ctrl_base.y_relative_parent;
  this->with = ctrl_base.with;
  this->height = ctrl_base.height;
  OutputDebugString(L"call move construct\n");
}

control_base& control_base::operator=(control_base& ctrl_base)
{
  this->childrens = ctrl_base.childrens;
  this->parent = ctrl_base.parent;
  this->x_relative_parent = ctrl_base.x_relative_parent;
  this->y_relative_parent = ctrl_base.y_relative_parent;
  this->with = ctrl_base.with;
  this->height = ctrl_base.height;
  
  OutputDebugString(L"call move assignment\n");
  // TODO: 在此处插入 return 语句
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
  OutputDebugString(L"call move assignment\n");
  // TODO: 在此处插入 return 语句
  return *this;
}

control_base::~control_base()
{
  std::wostringstream o;
  o << "~ " << this << " " << this->x_relative_parent << "\n";
}

void control_base::paint(HDC hdc)
{
  point p1(x_relative_parent, y_relative_parent);
  point p2(with + x_relative_parent, height + y_relative_parent);
  point relativep = globalposition();
  p1 += relativep;
  p2 += relativep;
  Rectangle(hdc,p1.x,p1.y , p2.x, p2.y);
  for (auto &c:childrens )
  {
    c.paint(hdc);
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
  childrens.push_back(control);
  auto& c = childrens.back();
  c.parent = this;
  return &childrens.back();
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
    std::wostringstream o;
    o << "push " <<this << " " << this->x_relative_parent << " p:" << this->parent << "\n";
    OutputDebugString(o.str().c_str());
  }
  for (auto &c:childrens)
  {
    auto child_ret = c.controlsAtPoint(p);
    for (auto c2:child_ret)
    {
      ret.push_back(c2);
    }
  }

  std::wostringstream o;
  o << this << "  ret------------------begin---" << "\n";
  for (auto c:ret)
  {
    o << "ret " << c << " " << c->x_relative_parent << " p:" << c->parent << "\n";
  }
  o << this << "  ret------------------end---" << "\n";
  OutputDebugString(o.str().c_str());

  return ret;
}

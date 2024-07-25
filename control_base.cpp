#include "control_base.h"
#include "structs.h"

control_base::control_base():
  x_relative_parent(0),
  y_relative_parent(0),
  parent(nullptr)
{
}

void control_base::paint(HDC hdc)
{
  point p1(x_relative_parent, y_relative_parent);
  point p2(with + x_relative_parent, height + y_relative_parent);
  point relativep = globalposition();
  p1 += relativep;
  p2 += relativep;
  Rectangle(hdc,p1.x,p1.y , p2.x, p2.y);
  for (auto c:childrens )
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
  childrens.push_back(std::move(control));
  childrens.back().parent = this;
  return &childrens.back();
}

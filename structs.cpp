#include "structs.h"

point::point():
  x(0),
  y(0)
{
}

point::point(int x, int y):x(x),y(y)
{
}

point point::operator+(point &a)
{
  return point(a.x+x,a.y+y);
}
//point& point::operator+=(point& a)
//{
//  x += a.x;
//  y += a.y;
//  return *this;
//}
point& point::operator+=(point a)
{
  x += a.x;
  y += a.y;
  return *this;
}

#include "structs.h"

point::point():
  x(0),
  y(0)
{
}

point::point(int x, int y):x(x),y(y)
{
}

point::point(const point& other)
{
  x = other.x;
  y = other.y;
}

point::point(point&& other)
{
  x = other.x;
  y = other.y;
}

point& point::operator=(const point& other)
{
  // TODO: 在此处插入 return 语句
  x = other.x;
  y = other.y;
  return *this;
}

point& point::operator=(point&& other)
{
  // TODO: 在此处插入 return 语句
  x = other.x;
  y = other.y;
  return *this;
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

rgb::rgb():r(255), g(255), b(255)
{

}

rgb::rgb(int r, int g, int b)
{
  this->r = r;
  this->g = g;
  this->b = b;
}

rgb::rgb(const rgb& other)
{
  r = other.r;
  g = other.g;
  b = other.b;
}

rgb::rgb(rgb&& other)
{
  r = other.r;
  g = other.g;
  b = other.b;
}

rgb& rgb::operator=(const rgb& other)
{
  r = other.r;
  g = other.g;
  b = other.b;
  // TODO: 在此处插入 return 语句
  return *this;
}

rgb& rgb::operator=(rgb&& other)
{
  r = other.r;
  g = other.g;
  b = other.b;
  // TODO: 在此处插入 return 语句
  return *this;
}

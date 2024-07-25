#ifndef STRUCTS
#define STRUCTS
class point
{
public:
  point();
  point(int x,int y);
  int x;
  int y;
  point operator + (point &a);
  //point& operator += (point& a);
  point& operator += (point a);
};

#endif


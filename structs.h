#ifndef STRUCTS
#define STRUCTS
class point
{
public:
  point();
  point(int x,int y);
  point(const point& other);
  point( point&& other);
  point& operator=(const point& other);
  point& operator=( point&& other);

  int x;
  int y;
  point operator + (point &a);
  //point& operator += (point& a);
  point& operator += (point a);
};

class rgb
{
public:
  rgb();
  rgb(int r, int g, int b);
  rgb(const rgb & other);
  rgb(rgb && other);
  rgb& operator=(const rgb& other);
  rgb& operator=( rgb&& other);
  
  int r; int g; int b;
};

#endif


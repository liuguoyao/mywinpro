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
  point& operator += (point a);
  point operator - (point& a);
  point& operator -= (point a);
};
using size = point;

class rgb
{
public:
  rgb();
  rgb(double r, double g, double b);
  rgb(const rgb & other);
  rgb(rgb && other);
  rgb& operator=(const rgb& other);
  rgb& operator=( rgb&& other);
  rgb&& operator-(const rgb& other);
  rgb&& operator+(const rgb& other);
  rgb&& operator*(const double num);
  rgb&& operator/(const double num);
  rgb& operator+=(const rgb& other);
  double abs();
  
  double r; double g; double b;
};

class evt
{
public:
  evt();
  evt(int x,int y,int type,int button);
  evt(const evt& other);
  evt& operator=(const evt& other);

  int x;
  int y;
  int type;
  int button;
};

struct sizePolicy
{
  int xPolicy;
  int yPolicy;
  int xFactor;
  int yFactor;
};

#endif


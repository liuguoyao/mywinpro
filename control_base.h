#ifndef CONTROL_BASE
#define CONTROL_BASE
#include "framework.h"
#include <vector>
#include "structs.h"

class control_base
{
public:
  control_base() ;
  control_base(const control_base & ctrl_base) ;
  control_base(const control_base && ctrl_base) ;
  control_base& operator=(control_base& ctrl_base);
  control_base& operator=(control_base&& ctrl_base);
  ~control_base();
  void paint(HDC hdc);
  void resize(int w,int h);
  void setposition(int x, int y);
  point position();
  point globalposition();
  control_base* addChild(control_base &control);
  bool containsPoint(const point &p);
  std::vector<control_base*> controlsAtPoint(const point& p);
  void setBkColor(const rgb &rgb );
private:
  int x_relative_parent;
  int y_relative_parent;
  int with;
  int height;
  std::vector<control_base> childrens;
  control_base* parent;
  rgb bkrgb;
};

#endif


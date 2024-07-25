#ifndef CONTROL_BASE
#define CONTROL_BASE
#include "framework.h"
#include <vector>
#include "structs.h"
class control_base
{
public:
  //control_base() = delete;
  control_base();
  void paint(HDC hdc);
  void resize(int w,int h);
  void setposition(int x, int y);
  point position();
  point globalposition();
  control_base* addChild(control_base &control);
private:
  int x_relative_parent;
  int y_relative_parent;
  int with;
  int height;
  std::vector<control_base> childrens;
  control_base* parent;
};

#endif


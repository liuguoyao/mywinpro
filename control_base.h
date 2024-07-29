#ifndef CONTROL_BASE
#define CONTROL_BASE
#include "framework.h"
#include <vector>
#include "structs.h"
#include <functional>
#include <string>
#include <set>

class control_base
{
public:
  control_base() ;
  control_base(std::wstring name) ;
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
  void updateState(const point &p);

  void setBkColor(const rgb &rgb );

  //mouse event
  std::function<void()> onEnter;
  std::function<void()> onLeave;
  std::function<void()> onClick;
  std::function<void()> onDoubleClick;

  //draw
  void onPaint(HDC hdc);

public:
  std::wstring name;
  int id;
  std::vector<control_base> childrens;

private:
  int x_relative_parent;
  int y_relative_parent;
  int with;
  int height;
  
  control_base* parent;
  rgb bkrgb;
  bool hover;

  //box model
  int margin;
  int borderWidth;
  int padding;

  

};

#endif


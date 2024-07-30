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
  control_base(const std::wstring &name, control_base* parent = nullptr) ;
  virtual ~control_base();
  void paint(HDC hdc);

  void resize(int w,int h);
  void setposition(int x, int y);
  point position();
  point globalposition();
  control_base* addChild(control_base *control);
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
  virtual void onPaint(HDC hdc);

  bool operator<(const control_base  &other) const;

public:
  std::wstring name;
  int id;
  std::set<control_base*> childrens;

protected:
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


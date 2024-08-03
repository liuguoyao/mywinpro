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
  control_base() {};
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

  // event process
  virtual bool processEvent(evt e);
  virtual void processMouseMove(const point &p);
  virtual void processLButtonDown();
  virtual void processLButtonUp();
  virtual void processLButtonDBLClick();

  virtual void updateState(long long delt_time);
  virtual void setBkColor(const rgb &rgb );

  //mouse event
  std::function<void(const point &)> onMouseMove;
  std::function<void()> onEnter;
  std::function<void()> onLeave;
  std::function<void()> onClick;
  std::function<void()> onDoubleClick;
  std::function<void()> onLButtonDown;
  std::function<void()> onLButtonUp;
  std::function<void()> onLButtonDBLClick;

  //draw
  virtual void onPaint(HDC hdc);
  virtual void onupdateAnimState(long long delt_time);

  void invalidate();

  bool operator<(const control_base  &other) const;

public:
  std::wstring name;
  int id;
  std::set<control_base*> childrens;

protected:
  RECT rect_global(); 

protected:
  int x_relative_parent;
  int y_relative_parent;
  int with;
  int height;
  
  control_base* parent;
  bool hover;

  //box model
  int margin;
  int borderWidth;
  int padding;

  //color
  rgb borderColor;
  rgb hoverColor;
  rgb activeColor;
  rgb backgroundColor;
  rgb fontColor;
  rgb curborderColor;
  rgb curhoverColor;
  rgb curactiveColor;
  rgb curbackgroundColor;
  rgb curfontColor;
  rgb deltaborderColor;
  rgb deltahoverColor;
  rgb deltaactiveColor;
  rgb deltabackgroundColor;
  rgb deltafontColor;

  // mouse state
  bool mouseLeftButtonDown;
  bool mouseRightButtonDown;
  bool mouseLeftButtonUp;
  bool mouseRightButtonUp;
  bool mouseLeftButtonClick;


private:
  bool needupdate;

};

#endif


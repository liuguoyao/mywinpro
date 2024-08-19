#ifndef CONTROL_BASE
#define CONTROL_BASE
#include "framework.h"
#include <vector>
#include "structs.h"
#include "global_var.h"
#include <functional>
#include <string>
#include <set>
#include <Windows.h>

class control_base
{

public:
  control_base() {};
  control_base(const std::wstring &name, control_base* parent = nullptr) ;
  virtual ~control_base();
  virtual std::wstring classtype() const;
  void paint(HDC hdc);

  void resize(int w,int h);
  void resize(size s);
  size getSize();
  void setposition(int x, int y);
  point position();
  point position_in_app();
  virtual control_base* addChild(control_base *control);
  bool containsPoint(const point &p);
  std::vector<control_base*> controlsAtPoint(const point& p);
  float text_width(HWND hWnd, std::wstring text);

  // event process
  virtual bool processEvent(evt e);
  virtual void processMouseMove(const point &p);
  virtual void processMouseEnter(evt e);
  virtual void processMouseLeave(evt e);
  virtual void processLButtonDown(evt e);
  virtual void processLButtonUp(evt e);
  virtual void processLButtonDBLClick(evt e);

  virtual void updateState(long long delt_time);
  virtual void setBkColor(const rgb &rgb );

  //mouse event
  std::function<void(const point &)> onMouseMove;
  std::function<void(evt)> onEnter;
  std::function<void(evt)> onLeave;
  std::function<void(evt)> onClick;
  std::function<void(evt)> onDoubleClick;
  std::function<void(evt)> onLButtonDown;
  std::function<void(evt)> onLButtonUp;
  std::function<void(evt)> onLButtonDBLClick;

  //draw
  virtual void onPaint(HDC hdc);
  virtual void onupdateAnimState(long long delt_time);

  //imm
  virtual void processIMMEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  //foucus
  virtual bool hasFocus();
  virtual void setFocus(bool focus);

  // layout
  virtual void placeChildren() {};
  sizePolicy getSizePolicy() const;
  void setSizePolicy(sizePolicy &sp);
  void setxPolicy(int xPolicy);
  void setyPolicy(int yPolicy);
  void setxFactor(int xFactor);
  void setyFactor(int yFactor);

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
  int width;
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

  //focus
  bool _hasFocus;

  //sizePolicy
  sizePolicy _sizePolicy;

private:
  bool needupdate;

};

#endif


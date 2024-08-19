#ifndef _H_APP_
#define _H_APP_

#include "framework.h"
#include "control_base.h"
#include "timer.h"
#include <vector>
#include <string>
#include <set>
#include <list>
#include "Ctrls.h"
#define APP app::getInstance()

class app
{
    public:
     app(app&)=delete;
     app(app&&)=delete;
     app& operator=(app&)=delete;
     app& operator=(app&&)=delete;
     static app& getInstance();

     int run();
     control_base* addChild(control_base* control);
     control_base* addLayout(layout* layout);
     void Invalidate();
     std::vector<control_base*> controlsAtPoint(const point &p);
     control_base* findControlByName(const std::wstring& name);
     size getSize() const;
     static void onSizeChanged(size newSize);

     //timer
     void remove_timer(timer *t);
     void add_timer(timer *t);

private:
  app();
  static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
  HWND hWnd;

private:
  HACCEL hAccelTable;
  UINT_PTR uTimerId;
  static long long last_update_time;
  static std::set<control_base*> childrens;
  static std::list<timer*> timers;
};

#endif // !

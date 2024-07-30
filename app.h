#pragma once
#include "framework.h"
#include "control_base.h"
#include <vector>
#include <string>
#include <set>

class app
{
    public:
      app();
     int run();
     control_base* addChild(control_base* control);
     void Invalidate();
     std::vector<control_base*> controlsAtPoint(const point &p);
     control_base* findControlByName(const std::wstring& name);

private:
  static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  BOOL WINAPI TranslateMessage( _In_ CONST MSG* lpMsg);
  int
    WINAPI
    TranslateAccelerator(
      _In_ HWND hWnd,
      _In_ HACCEL hAccTable,
      _In_ LPMSG lpMsg);
  LRESULT
    WINAPI
    DispatchMessage(
      _In_ CONST MSG* lpMsg);

 
private:
  HACCEL hAccelTable;
  HWND hWnd;
  static std::set<control_base*> childrens;
};

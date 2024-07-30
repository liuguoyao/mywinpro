#include "app.h"
#include "global_var.h"
#include "resource.h"
#include "framework.h"
#include <iostream>
#include <string>
#include <sstream>
#include<vector>
#include <stack>
#include <assert.h>  

std::set<control_base*> app::childrens{};
app::app()
{

  WNDCLASSEXW wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInst;
  wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MYWINPRO));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MYWINPRO);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  RegisterClassExW(&wcex);


  hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  
  hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_MYWINPRO));
}

int app::run()
{
  MSG msg;
  Invalidate();
  // 主消息循环:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

control_base* app::addChild(control_base* control)
{
  //childrens.push_back(control);
  //return &childrens.back();
  std::pair<std::set<control_base*>::iterator,bool> pair = childrens.emplace(control);
  if (pair.second)
  {
    return (control_base*) *pair.first;
  }
  else {
    MessageBox(hWnd,L"has same name of control",L"", MB_OK);
  }
  return nullptr;
}

void app::Invalidate()
{
  InvalidateRect(hWnd, NULL, FALSE);
}

std::vector<control_base*> app::controlsAtPoint(const point& p)
{
  std::vector<control_base*> children_contrains_point;
  for (const auto& c:childrens)
  {
    auto childrens_contain = ((control_base&)c).controlsAtPoint(p);

    for (auto &c2 : childrens_contain)
    {
      children_contrains_point.push_back(c2);
    }
  }
  return children_contrains_point;
}

LRESULT app::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // 分析菜单选择:
    switch (wmId)
    {
    case IDM_ABOUT:
      //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

#define use_double_buffering 
#ifdef use_double_buffering
    //双缓冲
    HDC hdcMem = CreateCompatibleDC(hdc);
    RECT rect;
    GetClientRect(hWnd, &rect);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    // 填充背景色（可选）  
    HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255)); // 白色背景  
    FillRect(hdcMem, &rect, hbr);
    DeleteObject(hbr);
#endif

    //draw children
    for (const auto& c:childrens)
    {
#ifdef use_double_buffering
      c->paint(hdcMem);
#else
      c.paint(hdc);
#endif
      
    }

#ifdef use_double_buffering
    // 将内存DC的内容复制到窗口DC  
    BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, SRCCOPY);
    // 清理  
    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    DeleteObject(hbmMem);
#endif

    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

BOOL __stdcall app::TranslateMessage(const MSG* lpMsg)
{
  std::wostringstream o;
  o << "TranslateMessage" << lpMsg->message << "\n";

  std::vector<control_base*> r;
  switch (lpMsg->message)
  {
  case WM_LBUTTONDOWN:
  //case WM_MOUSEMOVE:
    POINT pt = lpMsg->pt;
    ScreenToClient(hWnd, &pt); // 将屏幕坐标转换为窗口坐标  
    //r = controlsAtPoint(point(pt.x, pt.y));
    for (const auto& c:childrens)
    {
      c->updateState(point(pt.x, pt.y));
    }
    
    return true;
    //break;

  default:
    break;
  }
  Invalidate();


  return ::TranslateMessage( lpMsg);
}

int __stdcall app::TranslateAccelerator(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg)
{
  std::wostringstream o;
  o << "TranslateAccelerator" <<  std::hex <<lpMsg->message << "\n";

  return ::TranslateAccelerator(hWnd,  hAccTable,  lpMsg);
}

LRESULT __stdcall app::DispatchMessage(const MSG* lpMsg)
{
  std::wostringstream o;
  o << "DispatchMessage" << lpMsg->message << "\n";
  return ::DispatchMessage(lpMsg);
}

control_base* app::findControlByName(const std::wstring& name)
{
  std::stack<control_base*> stack;
  for (const auto& c : childrens)
  {
    stack.push((control_base*) & c);
  }

  while (!stack.empty())
  {
    control_base* cur = stack.top();
    stack.pop();
    if (name != cur->name) {
      for (const auto& c1 : cur->childrens)
      {
        stack.push((control_base* ) & c1);
      }
    }
    else
    {
      return cur;
    }
  }
  MessageBox(NULL, (L"has no control name "+name).c_str(), L"错误", MB_ICONERROR);
  return nullptr;
}

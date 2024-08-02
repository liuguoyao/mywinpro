#include "app.h"
#include "global_var.h"
#include "resource.h"
#include "framework.h"
#include <iostream>
#include <string>
#include <sstream>
#include<vector>
#include <stack>
#include <chrono>

std::set<control_base*> app::childrens{};
long long app::last_update_time = 0;
app::app()
{

  WNDCLASSEXW wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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

  uTimerId = SetTimer(hWnd, 0, 40, NULL); //25hz

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  
  hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_MYWINPRO));

}

app& app::getInstance()
{
  static app instance;
  return instance;
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
  KillTimer(hWnd, uTimerId);
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
  case WM_LBUTTONDOWN:
    //OutputDebugString(L"WM_LBUTTONDOWN\n");
    break;
  case WM_LBUTTONUP:
    //OutputDebugString(L"WM_LBUTTONUP\n");
    break;
  case WM_LBUTTONDBLCLK:
    OutputDebugString(L"WM_LBUTTONDBLCLK WndProc\n");
    break;
  case WM_MOUSEMOVE:
  {
    POINT pt{ LOWORD(lParam),HIWORD(lParam) };
    //ScreenToClient(hWnd, &pt); // 将屏幕坐标转换为窗口坐标  
    for (const auto& c : childrens)
    {
      c->updateMousePosition(point(pt.x, pt.y));
    }
  }
   break;
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
  case WM_TIMER: {
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    auto delta_time = seconds - last_update_time;
    last_update_time = seconds;
    for (const auto& c : childrens)
    {
      c->updateState(delta_time);
    }
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

control_base* app::findControlByName(const std::wstring& name)
{
  std::stack<control_base*> stack;
  for (const auto c : childrens)
  {
    stack.push(c);
  }

  while (!stack.empty())
  {
    control_base* cur = stack.top();
    stack.pop();
    if (name != cur->name) {
      for (const auto c1 : cur->childrens)
      {
        stack.push(c1);
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

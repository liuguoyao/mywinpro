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
#include <imm.h>

std::set<control_base*> app::childrens{};
long long app::last_update_time = 0;
app::app()
{

  WNDCLASSEXW wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS| CS_DROPSHADOW;
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

control_base* app::addLayout(layout* layout)
{
  layout->resize(getSize());
  return addChild(layout);
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
    auto childrens_contain = c->controlsAtPoint(p);

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
  case WM_SIZE:
  {
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    onSizeChanged(size(width,height));
    //return DefWindowProc(hWnd, message, wParam, lParam);
    break;
  }
  case WM_LBUTTONUP:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONDBLCLK:
  case WM_MOUSEMOVE:
    for (const auto& c : childrens)
      c->processEvent(evt(LOWORD(lParam), HIWORD(lParam), message, (int)wParam));
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
    HFONT hFont = CreateFont(
      20,              // 字体高度
      0,               // 字体宽度，0表示默认
      0,               // 文本旋转角度
      0,               // 字体倾斜角度
      FW_NORMAL,       // 字重
      FALSE,           // 粗体
      FALSE,           // 下划线
      FALSE,           // strikeout（删除线）
      DEFAULT_CHARSET, // 字符集
      OUT_DEFAULT_PRECIS, // 输出精度
      CLIP_DEFAULT_PRECIS, // 剪辑精度
      CLEARTYPE_QUALITY, // 输出质量
      DEFAULT_PITCH | FF_DONTCARE, // 间距和字体家族
      L"JetBrains Mono NL Medium"         // 字体名
    );
    
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
    auto oldfont = SelectObject(hdcMem, hFont);
#else
    auto oldfont = SelectObject(hdc, hFont);
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
    SelectObject(hdc, oldfont);
    DeleteObject(hFont);
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

  //imm
  case WM_IME_SETCONTEXT:
    return DefWindowProc(hWnd, message, wParam, lParam);
  case WM_IME_STARTCOMPOSITION:
  case WM_IME_ENDCOMPOSITION:
  case WM_IME_COMPOSITION:
  case WM_CHAR: //keyboard 
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_DEADCHAR:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
    for (const auto& c : childrens)
    {
      c->processIMMEvent(hWnd, message, wParam, lParam);
    }
    break;

  //focus
  case WM_MY_SETFOCUS:{
    std::stack<control_base*> stack;
    for (const auto& c : childrens)
    {
      stack.emplace(c);
    }

    while (!stack.empty())
    {
      auto item = stack.top();
      stack.pop();
      for (const auto& c : item->childrens)
      {
        stack.emplace(c);
      }
      if (item!=(control_base*)wParam)
      {
        item->setFocus(false);
      }
    }
  }
    break;
  //case WM_MY_KILLFOCUS:
  //  break;

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

size app::getSize() const
{
  RECT rect;
  GetWindowRect(hWnd, &rect);
  return size(rect.right - rect.left, rect.bottom - rect.top);
}

void app::onSizeChanged(size newSize)
{
  for (auto c : childrens )
  {
    if (L"hlayout"==c->classtype()) {
      c->resize(newSize);
    }
  }
}

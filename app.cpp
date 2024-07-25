#include "app.h"
#include "global_var.h"
#include "resource.h"
#include "framework.h"
#include <iostream>
#include <string>
#include <sstream>

std::vector<control_base> app::childrens{};
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
  // ����Ϣѭ��:
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

control_base* app::addChild(control_base& control)
{
  childrens.push_back(std::move(control));
  return &childrens.back();
}

void app::Invalidate()
{
  InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT app::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // �����˵�ѡ��:
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
    // TODO: �ڴ˴�����ʹ�� hdc ���κλ�ͼ����...
    // ���û�����ɫΪ��ɫ  
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    //draw children
    for (auto c:childrens)
    {
      c.paint(hdc);
    }

    // �ָ��ɻ���  
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

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
  OutputDebugString(o.str().c_str());
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
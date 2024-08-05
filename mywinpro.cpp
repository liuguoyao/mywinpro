// mywinpro.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "mywinpro.h"
#include "global_var.h"

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
int nCmdShow;

//color 
rgb borderColor(0x40, 0x51, 0x4e);
rgb hoverColor(0xe4, 0xf9, 0xf5);
rgb clickDownColor( 0x11, 0x99, 0x9e);
rgb activeColor(0x30, 0xe3, 0xca);
rgb backgroundColor(0xf9, 0xf7, 0xf7);
rgb fontColor(0x40, 0x51, 0x4e);
rgb focusBorderColor(0x11, 0x99, 0x9e);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
  ::hInst = hInstance;
  ::nCmdShow = nCmdShow;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYWINPRO, szWindowClass, MAX_LOADSTRING);
    return main(0,NULL);
}


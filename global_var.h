#pragma once
#define MAX_LOADSTRING 100

#include "structs.h"

// 全局变量:
extern HINSTANCE hInst;                                // 当前实例
extern WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
extern WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
extern int nCmdShow;

int main(int argc, char** argv);

//color
extern rgb borderColor;
extern rgb hoverColor;
extern rgb clickDownColor;
extern rgb activeColor;
extern rgb backgroundColor;
extern rgb fontColor;
extern rgb focusBorderColor;

//sizepolicy
#define SIZEPOLICY_FIXED 0
#define SIZEPOLICY_EXPAND 1


//custom vm
#define WM_MY_SETFOCUS (WM_USER + 1)
#define WM_MY_KILLFOCUS (WM_USER + 2)

#pragma once
#define MAX_LOADSTRING 100

// 全局变量:
extern HINSTANCE hInst;                                // 当前实例
extern WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
extern WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
extern int nCmdShow;

int main(int argc, char** argv);

#pragma once
#define MAX_LOADSTRING 100

#include "structs.h"

// ȫ�ֱ���:
extern HINSTANCE hInst;                                // ��ǰʵ��
extern WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
extern WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
extern int nCmdShow;

int main(int argc, char** argv);

//color
extern rgb borderColor;
extern rgb hoverColor;
extern rgb activeColor;
extern rgb backgroundColor;
extern rgb fontColor;

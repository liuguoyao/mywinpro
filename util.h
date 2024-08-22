#ifndef _H_UTIL_H
#define _H_UTIL_H
#include <Windows.h>
#include <vector>
#include <iostream>

bool get_processname(std::wstring name);
bool KillProcess(const std::wstring& processName);

bool isAsciiChar(const char& c);
bool isAsciiWchar(const WCHAR& c);

std::vector<bool> checkAsciiInWString(const std::wstring& ws);

#endif // !_H_UTIL_H



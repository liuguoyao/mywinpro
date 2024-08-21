#ifndef _H_UTIL_H
#define _H_UTIL_H
#include <Windows.h>
#include <vector>
#include <iostream>

bool isAsciiChar(const char& c);
bool isAsciiWchar(const WCHAR& c);

std::vector<bool> checkAsciiInWString(const std::wstring& ws);

#endif // !_H_UTIL_H



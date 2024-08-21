#include "util.h"

bool isAsciiChar(const char& c) {
  // ��鵥���ֽ��Ƿ���ASCII��Χ��  
  return (static_cast<unsigned char>(c)) <= 0x7F;
}
bool isAsciiWchar(const WCHAR& c) {
  // ��鵥���ֽ��Ƿ���ASCII��Χ��  
  return (static_cast<unsigned short>(c)) <= 0x7F;
}

std::vector<bool> checkAsciiInWString(const std::wstring& ws) {
  std::vector<bool> result;
  for (wchar_t wc : ws) {
    result.push_back(isAsciiWchar(wc));
  }
  return result;
}
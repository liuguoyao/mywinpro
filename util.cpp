#include "util.h"
#include <tchar.h>
#include <psapi.h>  
#include <tlhelp32.h>  
#include <windows.h>
#pragma comment(lib, "psapi.lib")  

bool KillProcess(const std::wstring& processName) {
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot == INVALID_HANDLE_VALUE) {
    return false;
  }

  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hSnapshot, &pe32)) {
    CloseHandle(hSnapshot);
    return false;
  }

  do {
    if (wcscmp(pe32.szExeFile, processName.c_str()) == 0) {
      HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
      if (hProcess != NULL) {
        if (!TerminateProcess(hProcess, 0)) {
        }
        CloseHandle(hProcess);
        CloseHandle(hSnapshot);
        return true;
      }
      else {
      }
    }
  } while (Process32Next(hSnapshot, &pe32));

  CloseHandle(hSnapshot);
  return false;
}

bool get_processname(std::wstring name) {
  DWORD aProcesses[1024], cbNeeded, cProcesses;
  unsigned int i;

  if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    return false;

  cProcesses = cbNeeded / sizeof(DWORD);

  for (i = 0; i < cProcesses; i++) {
    if (aProcesses[i] != 0) {
      HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

      if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
          TCHAR szProcessName[MAX_PATH];

          // 获取模块（通常是主执行文件）的完整路径  
          if (GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR))) {
            // 转换为小写进行比较（可选，取决于你的需求）  
            for (int j = 0; szProcessName[j]; j++) {
              szProcessName[j] = tolower(szProcessName[j]);
            }
            if (_tcscmp(szProcessName, name.c_str()) == 0) {
              CloseHandle(hProcess);
              return true;
            }
          }
        }
        CloseHandle(hProcess);
      }
    }
  }
  return false;
}

bool isAsciiChar(const char& c) {
  // 检查单个字节是否在ASCII范围内  
  return (static_cast<unsigned char>(c)) <= 0x7F;
}
bool isAsciiWchar(const WCHAR& c) {
  // 检查单个字节是否在ASCII范围内  
  return (static_cast<unsigned short>(c)) <= 0x7F;
}

std::vector<bool> checkAsciiInWString(const std::wstring& ws) {
  std::vector<bool> result;
  for (wchar_t wc : ws) {
    result.push_back(isAsciiWchar(wc));
  }
  return result;
}
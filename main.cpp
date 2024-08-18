#include "app.h"
#include "control_base.h"
#include "global_var.h"
#include <sstream>
#include "label.h"
#include "Ctrls.h"
#include <commdlg.h>  
#include "edit.h"
#include <tlhelp32.h>  

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

int main(int argc,char ** argv) {
  app& a = app::getInstance();

  // -------------------- vlayout ----------------
  auto vl = Ctrls::instance().create_vlayout(L"vlayout", nullptr);
  auto btn_fresh = Ctrls::instance().create_button(L"ˢ��", vl);
  btn_fresh->setxPolicy(SIZEPOLICY_EXPAND);
  auto hl = Ctrls::instance().create_hlayout(L"hlayout", vl);
  hl->setyPolicy(SIZEPOLICY_FIXED);
  auto btn1 = Ctrls::instance().create_button(L"button", hl);
  auto edit = Ctrls::instance().create_edit(L"edit", hl);
  edit->set_text(L"notepad++.exe");
  auto btn_endp = Ctrls::instance().create_button(L"�ս�", hl);
  //auto hl2 = Ctrls::instance().create_hlayout(L"hlayout2", vl);
  //hl2->setyPolicy(SIZEPOLICY_FIXED);
  //auto f2 = Ctrls::instance().create_button(L"button2", hl2);
  //auto g2 = Ctrls::instance().create_edit(L"edit2", hl2);
  //g2->setyPolicy(SIZEPOLICY_EXPAND);
  //auto h2 = Ctrls::instance().create_button(L"button22", hl2);
  //h2->setyPolicy(SIZEPOLICY_EXPAND);
  a.addLayout(vl);
  // -----------------------------------------

  //-------- process
  btn1->onLButtonDown = [&](evt e) {
    OPENFILENAME ofn;
    wchar_t szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = APP.hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

    // ��ʾ���ļ��Ի���  
    if (GetOpenFileName(&ofn) == TRUE)
    {
      
      edit->set_text(std::wstring(szFile));
    }
    };

  btn_endp->onLButtonDown = [&](evt e) {
    OutputDebugString((L"endprocess: " + edit->get_text()).c_str());
    KillProcess(edit->get_text());
    };
  //-----------------

  // -------------------- hlayout ----------------
  //auto hl = Ctrls::instance().create_hlayout(L"hlayout", nullptr);
  //auto vl = Ctrls::instance().create_vlayout(L"vlayout", hl);
  //auto vl2 = Ctrls::instance().create_vlayout(L"vlayout2", hl);

  //auto f = Ctrls::instance().create_button(L"button", vl);
  //auto g = Ctrls::instance().create_edit(L"edit", vl);
  //g->setxPolicy(SIZEPOLICY_EXPAND);
  //f->setxPolicy(SIZEPOLICY_EXPAND);
  //auto h = Ctrls::instance().create_button(L"button2", vl);
  //h->setxPolicy(SIZEPOLICY_EXPAND);
  //auto f2 = Ctrls::instance().create_button(L"button1", vl2);
  //auto g2 = Ctrls::instance().create_edit(L"edit2", vl2);
  //auto h2 = Ctrls::instance().create_button(L"button12", vl2);
  //a.addLayout(hl);
  // -----------------------------------------

  return a.run();
}
#include "app.h"
#include "control_base.h"
#include "global_var.h"
#include <sstream>
#include "label.h"
#include "Ctrls.h"
#include <commdlg.h>  
#include "edit.h"
//#include <tlhelp32.h>  
#include <windows.h>
#include <shellapi.h>
#include "util.h"

int main(int argc,char ** argv) {
  app& a = app::getInstance();

  // -------------------- vlayout ----------------
  auto vl = Ctrls::instance().create_vlayout(L"vlayout", nullptr);
  auto btn_fresh = Ctrls::instance().create_button(L"刷新", vl);
  btn_fresh->setxPolicy(SIZEPOLICY_EXPAND);
  auto hl = Ctrls::instance().create_hlayout(L"hlayout", vl);
  hl->setyPolicy(SIZEPOLICY_FIXED);
  auto btn1 = Ctrls::instance().create_button(L"selectfile", hl);
  btn1->resize(btn1->getSize().x * 2, btn1->getSize().y);
  auto btn_create = Ctrls::instance().create_button(L"创", hl);
  auto edit = Ctrls::instance().create_edit(L"edit", hl);
  edit->set_text(L"notepad++.exe");
  auto btn_endp = Ctrls::instance().create_button(L"毙", hl);
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
  timer t(1000, CYCLE, [&]() {
    if (get_processname(edit->get_text()))
    {
      btn1->setBkColor(textSelectedBgColor);
    }
    else
    {
      btn1->setBkColor(alertColor);
    }
    });
  btn_fresh->onLButtonDown = [&](evt e) {
    if(t.is_running())
      t.stop();
    else
      t.start();
    btn_fresh->set_text(t.is_running() ? L"停止" : L"刷新");
    };

  btn_create->onLButtonDown = [&](evt e) {
    std::wstring filename = L"notepad++.exe";
    std::wstring dir = L"C:/Program Files(x86)/Notepad++";
    HINSTANCE hInstance = ShellExecute(
      NULL,L"open", filename.c_str(),
      NULL,dir.c_str(),SW_SHOWNORMAL);
    };

  btn1->onLButtonDown = [&](evt e) {
    OPENFILENAME ofn;
    wchar_t szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = APP.hWnd;
    //ofn.lpstrFile = szFile;
    ofn.lpstrFile = L'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    //ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;

    // 显示打开文件对话框  
    OutputDebugString(L"OpenFile: ");
    if (GetOpenFileName(&ofn) == TRUE)
    {
      
      edit->set_text(std::wstring(szFile));
    }
    OutputDebugString(L"end OpenFile: ");
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
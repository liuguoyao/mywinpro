#ifndef EDIT_H
#define EDIT_H
#include "control_base.h"
class edit :
    public control_base
{
public:
  edit(std::wstring name, control_base* parent = nullptr);

  virtual void onPaint(HDC hdc) override;
  virtual void onupdateAnimState(long long delta_time) override;
  virtual void processLButtonDown(evt e) override;
  virtual void processLButtonUp(evt e) override;
  virtual void processLButtonDBLClick(evt e) override;
  virtual void processMouseEnter(evt e) override;
  virtual void processMouseLeave(evt e) override;
  virtual void processMouseMove(const point& p) override;
  virtual void setFocus(bool focus) override;

  virtual void processIMMEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
  virtual bool processEvent(evt e) override;

  void set_text(const std::wstring& text);
  std::wstring get_text() const;

 private:
    //point text_cursor_pos;
   unsigned short _time_acc;
   bool _draw_text_cursor;
   float _text_selectd_start_pos;
   float _text_selectd_end_pos;
   std::wstring _comtext;
   std::wstring _context;
   float _pos_text_cursor;

};

#endif
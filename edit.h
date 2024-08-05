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
  virtual void processLButtonDown() override;
  virtual void processLButtonUp() override;
  virtual void processLButtonDBLClick() override;
  virtual void processMouseEnter() override;
  virtual void processMouseLeave() override;
  virtual void setFocus(bool focus) override;


  void set_text(const std::wstring& text);
  std::wstring get_text() const;

 private:
    //point text_cursor_pos;
   unsigned short _time_acc;
   bool _draw_text_cursor;


};

#endif
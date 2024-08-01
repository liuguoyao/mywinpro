#ifndef BUTTON_H
#define BUTTON_H

#include "control_base.h"
class button :
    public control_base
{
public:
  button();
  button(std::wstring name, control_base* parent=nullptr);
  virtual void onPaint(HDC hdc);
  virtual void onupdateAnimState(long long delta_time) override;
  void invalidate();

private:
  RECT rect_global();
  UINT_PTR timerid;
};


#endif // BUTTON_H


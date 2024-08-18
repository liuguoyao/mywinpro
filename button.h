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
  virtual void processLButtonDown(evt e) override;
  virtual void processLButtonUp(evt e) override;
  virtual void processLButtonDBLClick(evt e) override;
  virtual void processMouseEnter(evt e);
  virtual void processMouseLeave(evt e);

private:

};


#endif // BUTTON_H


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

};

#endif // BUTTON_H


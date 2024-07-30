#ifndef LABEL_H
#define LABEL_H
#include "control_base.h"

class label :
    public control_base
{
public:
  label();
  label(const std::wstring &name);
  label(const control_base& other);
  label(const control_base&& other);
  void onPaint(HDC hdc) override;

};
#endif // LABEL_H


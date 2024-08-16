#include "layout.h"

layout::layout()
{
}


layout::layout(std::wstring name, control_base* parent)
  : control_base(name, parent)
{
  _sizePolicy.xPolicy = SIZEPOLICY_EXPAND;
  _sizePolicy.yPolicy = SIZEPOLICY_EXPAND;
  _sizePolicy.xFactor= 1;
  _sizePolicy.yFactor = 1;
}
layout::~layout(){}



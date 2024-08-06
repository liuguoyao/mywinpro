#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include "control_base.h"

//主要功能:控件自动伸缩, 自动布局,控制控件边距,自动对齐
class layout :
    public control_base
{
public:
    layout();
    layout(std::wstring name, control_base* parent=nullptr);
    virtual ~layout();

};

#endif


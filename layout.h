#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include "control_base.h"

//��Ҫ����:�ؼ��Զ�����, �Զ�����,���ƿؼ��߾�,�Զ�����
class layout :
    public control_base
{
public:
    layout();
    layout(std::wstring name, control_base* parent=nullptr);
    virtual ~layout();

};

#endif


#pragma once
#include "layout.h"
class hlayout :
    public layout
{
public:
    hlayout();
    hlayout(const std::wstring& name, control_base* parent = nullptr);
    virtual ~hlayout(); 
    virtual std::wstring classtype() const;
    virtual control_base* addChild(control_base* control) override;
    virtual bool processEvent(evt e);
    virtual void placeChildren() override;
};


#ifndef _V_VLAYOUT_
#define _V_VLAYOUT_

#include "layout.h"
class vlayout :
    public layout
{
public:
  vlayout();
  vlayout(const std::wstring& name, control_base* parent = nullptr);
  virtual ~vlayout();
  virtual std::wstring classtype() const;
  virtual control_base* addChild(control_base* control) override;
  virtual bool processEvent(evt e);
  virtual void placeChildren() override;
};

#endif // !_V_VLAYOUT_


#include "vlayout.h"
#include "app.h"

vlayout::vlayout()
{
}

vlayout::vlayout(const std::wstring& name, control_base* parent)
  : layout(name, parent)
{
}

vlayout::~vlayout()
{
}

std::wstring vlayout::classtype() const
{
  return L"vlayout";
}

control_base* vlayout::addChild(control_base* control)
{
  auto r = control_base::addChild(control);
  return r;
}


bool vlayout::processEvent(evt e)
{
  for (auto c : childrens)
  {
    c->processEvent(e);
  }
  return false;
}

void vlayout::placeChildren()
{
  if (nullptr == parent) {
    resize(APP.getSize());
  }
  else {
    resize(width, parent->getSize().y);
  }

  float margin = 2;
  float layout_height = (float)getSize().y;

  if (0 == layout_height) return;

  float curx = margin;
  float cury = margin;
  float acc_height_fixed = margin;
  float acc_height_factor = 0;
  int cnt_expend_children = 0;
  for (auto c : childrens)
  {
    auto policy = c->getSizePolicy();
    if (SIZEPOLICY_FIXED == policy.yPolicy)
    {
      acc_height_fixed += c->getSize().y+ margin;
    }
    else if (SIZEPOLICY_EXPAND == policy.yPolicy)
    {
      cnt_expend_children += 1;
      acc_height_factor += policy.yFactor;
    }
    if (SIZEPOLICY_EXPAND == policy.xPolicy) {
      c->resize(getSize().x - 2 * (int)margin, c->getSize().y);
    }
  }
  float height_expend_children = layout_height - acc_height_fixed - margin*(cnt_expend_children+1);
  for (auto c : childrens)
  {
    c->setposition((int)curx, (int)cury);
    if (SIZEPOLICY_FIXED == c->getSizePolicy().yPolicy)
    {
      cury += (c->getSize().y + margin);
    }
    else if (SIZEPOLICY_EXPAND == c->getSizePolicy().yPolicy)
    {
      float h = 0;
      if (0 == acc_height_factor)
      {
        h = height_expend_children / cnt_expend_children;
      }
      else
      {
        h = height_expend_children * c->getSizePolicy().yFactor / acc_height_factor;
      }
      cury += h + margin;
      c->resize(c->getSize().x,(int)h);
    }

  }
}

#include "hlayout.h"

hlayout::hlayout()
{
}

hlayout::hlayout(const std::wstring& name, control_base* parent)
  : layout(name, parent)
{
}

hlayout::~hlayout()
{
}

std::wstring hlayout::classtype() const
{
  return L"hlayout";
}

control_base* hlayout::addChild(control_base* control)
{
  auto r = control_base::addChild(control);
  placeChildren();
  return r;
}


bool hlayout::processEvent(evt e)
{
  for (auto c : childrens)
  {
    c->processEvent(e);
  }
  return false;
}

void hlayout::placeChildren()
{
  float margin = 2;
  float layout_width = (float)getSize().x;
  if (0 == layout_width) return;

  float curx = margin;
  float cury = margin;
  float acc_width_fixed = 0;
  float acc_width_factor = 0;
  int cnt_expend_children = 0;
  for (auto c : childrens)
  {
    auto policy = c->getSizePolicy();
    if (SIZEPOLICY_FIXED == policy.xPolicy)
    {
      acc_width_fixed+= c->getSize().x;
    }
    else if(SIZEPOLICY_EXPAND == policy.xPolicy)
    {
      cnt_expend_children += 1;
      acc_width_factor+= policy.xFactor;
    }
  }
  float width_expend_children = layout_width - acc_width_fixed - margin * (childrens.size()+1);
  for (auto c : childrens)
  {
    c->setposition((int)curx, (int)cury);
    if (SIZEPOLICY_FIXED == c->getSizePolicy().xPolicy)
    {
      curx += (c->getSize().x + margin);
    }
    else if (SIZEPOLICY_EXPAND == c->getSizePolicy().xPolicy)
    {
      float w =0;
      if (0 == acc_width_factor)
      {
        w= width_expend_children / cnt_expend_children ;
      }
      else
      {
        w = width_expend_children * c->getSizePolicy().xFactor / acc_width_factor;
      }
      curx += w + margin;
      c->resize((int)w, c->getSize().y);
    }
    
  }
}

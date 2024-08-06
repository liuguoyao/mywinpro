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
  int margin = 2;
  auto layout_width = getSize().x;
  if (0 == layout_width) return;

  int curx = margin;
  int cury = margin;
  int acc_width_fixed = 0;
  int acc_width_factor = 0;
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
  int width_expend_children = layout_width/2 - acc_width_fixed/2 - margin * (childrens.size()+1);
  //width_expend_children /= 2;
  for (auto c : childrens)
  {
    c->setposition(curx, cury);
    if (SIZEPOLICY_FIXED == c->getSizePolicy().xPolicy)
    {
      curx += (c->getSize().x / 2 + margin);
    }
    else if (SIZEPOLICY_EXPAND == c->getSizePolicy().xPolicy)
    {
      int w =0;
      if (0 == acc_width_factor)
      {
        w= width_expend_children / cnt_expend_children ;
        curx += w + margin;
        
      }
      else
      {
        w = width_expend_children * c->getSizePolicy().xFactor / acc_width_factor;
        curx += w + margin;
      }
      c->resize(w*2, c->getSize().y);
    }
    
  }

}
